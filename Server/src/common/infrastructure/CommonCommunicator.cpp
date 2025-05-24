#include "CommonCommunicator.h"

#include <iostream>

#include <thread>

#include "Constants.h"

#include "exception/SocketDisconnectionException.h"
#include "exception/SocketTimeoutException.h"

#include "handler/codec/s2c/response/Response.h"

#include "../handler/codec/c2s/request/JsonRequestPacketDeserializer.h"
#include "../handler/codec/s2c/response/JsonResponsePacketSerializer.h"
#include "handler/codec/s2c/notification/NotificationPacketSerializer.h"

#ifdef _WIN32
#else
#include <arpa/inet.h>
#endif


CommonCommunicator::CommonCommunicator(const SOCKET defaultSocket, const RequestHandlerFactory *const handlerFactory) :
    _running(false),
    _serverSockAddr({}),
    m_serverSocket(defaultSocket),
    m_handlerFactory(*handlerFactory)
{}

CommonCommunicator::~CommonCommunicator()
{
    close();
}

bool CommonCommunicator::isRunning() const
{
    return this->_running;
}

void CommonCommunicator::bindAndListen()
{
    commonSetup();
    startServerThreads();
}

void CommonCommunicator::close()
{
    if (!this->_running)
        return;

    // Notify all threads that the server is closing
    this->_running = false;

    // Release the client cleaner thread
    this->_disconnectedClientConditionalVariable.notify_all();

    // Wait for 'em to close
    this->m_clientsMutex.lock();
    for (const auto& client : this->m_clients)
    {
        client.second->getThread().wait();
    }
    this->m_clientsMutex.unlock();

    platformClose();

    this->_serverSockAddr = {};
    this->_serverThread = std::future<void>();
}

void CommonCommunicator::commonSetup()
{
    this->m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (!isValidSocket(this->m_serverSocket))
    {
        close();
        throwPlatformError("Socket creation failed");
        throw std::exception();
    }

    // The timeout for the recv method
    // Set in place to allow refreshing the value of _running.
    //TODO: Make it so that the client is required to send a ping every RECV_REFRESH_TIMEOUT_MS,
    // otherwise disconnect.
    // This helps against the phantom ports issues of C#.
    setRecvTimeout(RECV_REFRESH_TIMEOUT_MS);

    // Set server address information
    this->_serverSockAddr.sin_family = AF_INET;
    this->_serverSockAddr.sin_addr.s_addr = INADDR_ANY;
    this->_serverSockAddr.sin_port = htons(PORT);

    if (!isValidBind(
        bind(this->m_serverSocket, reinterpret_cast<sockaddr *>(&this->_serverSockAddr), sizeof(this->_serverSockAddr))
    )) {
        close();
        throwPlatformError("Binding failed");
        throw std::exception();
    }

    this->_running = true;

    // Start listening for connections
    if (!isValidListen(listen(this->m_serverSocket, 3)))
    {
        close();
        throwPlatformError("Listen failed");
        throw std::exception();
    }
}

void CommonCommunicator::registerClient(const SOCKET socket)
{
    Client* client = this->m_clients[socket] = new Client(
        socket,
        this->m_handlerFactory.createLoginRequestHandler()
    );

    client->setAndStartThread(
        [this, socket]()
        {
            _clientThreadFunc(socket);
        }
    );

#ifdef _WIN32
    std::cout << "Connection accepted from " + std::to_string(socket);
#else
    // Get the IP of the remote to display it
    socklen_t addrLen = sizeof(this->_serverSockAddr);
    char ipStr[INET_ADDRSTRLEN] = {};

    if (getpeername(socket, reinterpret_cast<sockaddr*>(&this->_serverSockAddr), &addrLen) == 0)
    {
        inet_ntop(AF_INET, &this->_serverSockAddr.sin_addr, ipStr, sizeof(ipStr));
    }

    std::cout << "Connection accepted from " + std::to_string(socket) << " (" << ipStr << ")" << std::endl;
#endif
}

void CommonCommunicator::startServerThreads()
{
    this->_serverThread = std::async(
        std::launch::async,
        [this]()
        {
            _serverThreadFunc();
        }
    );

    // Also start client cleaner thread
    std::thread(
        [this]()
        {
            _clientCleanerThreadFunc();
        }
    ).detach();

    std::cout << "Listening on port " << PORT << "..." << std::endl;
}

void CommonCommunicator::sendMsg(Client& client, const OBuffer& buffer) const
{
    bool didError;

    std::unique_lock<std::mutex> writerLock = client.acquireSocketWriterLock();
    try
    {
        // Casting for crybaby Windows
        // ReSharper disable once CppRedundantCastExpression
        didError = send(client.socket, reinterpret_cast<const char *>(buffer.contents), buffer.length, 0) == -1;
    }
    catch (...)
    {
        didError = true;
    }
    writerLock.unlock();

    if (didError)
    {
        throwPlatformError("Failed to send message to client socket " + std::to_string(client.socket));
        throw std::exception();
    }
}

void CommonCommunicator::throwPlatformError(const std::string& msg) const
{
    throw std::runtime_error(msg);
}

void CommonCommunicator::_serverThreadFunc()
{
    while (this->_running)
    {
        try
        {
            acceptClients();
        }
        catch (const std::exception& e)
        {
            std::cout << "Uncaught exception in server thread: " << e.what() << std::endl;
        }
    }

    close();
}

void CommonCommunicator::_clientThreadFunc(const SOCKET socket)
{
    while (this->_running)
    {
        try
        {
            _handleClient(socket);
        }
        catch (const SocketTimeoutException&)
        {
            // If we timed out (see RECV_REFRESH_TIMEOUT),
            // simply wait for the next recv cycle (if applicable).
            continue;
        }
        catch (const SocketDisconnectionException&)
        {
            break;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Unknown exception occurred (" << e.what() << "); Assuming client disconnection" << std::endl;
            break;
        }
    }

    this->m_clients.at(socket)->handleDisconnecting();

    _enqueueDisconnectClient(socket);
}

//ANCHOR Actual client processing function.
void CommonCommunicator::_handleClient(const SOCKET socket)
{
    const RequestInfo info = _waitForClientRequest(socket);

    std::unique_lock clientsLock(this->m_clientsMutex);
    Client& client = *this->m_clients.at(socket);
    clientsLock.unlock();

    std::unique_lock<std::mutex> handlerLock = client.acquireRequestHandlerLock();
    const IRequestHandler *const handler = client.getRequestHandler();

    if (!handler->isRequestRelevant(info))
    {
        _dispatchResponse(client, ErrorResponse(ErrorStatus::ILLEGAL_REQUEST, info.id));
        return;
    }

    const ProtocolRequest *request = nullptr;
    try
    {
        request = ProtocolRequest::fromRequest(info);
    } catch (const std::invalid_argument &)
    {
        _dispatchResponse(client, ErrorResponse(ErrorStatus::ILLEGAL_REQUEST, info.id));
        return;
    }

    const RequestResult *result = nullptr;

    try
    {
        result = new RequestResult(handler->handleRequest(info, *request));
    }
    catch (const std::exception &)
    {
        delete request;
        throw;
    }

    delete request;

    // The Handler did its job well.
    // 🫡
    delete handler;
    client.setRequestHandler(result->newHandler);
    handlerLock.unlock();

    _dispatchResponse(client, *result->response);
    delete result;
}

void CommonCommunicator::_dispatchResponse(Client &client, const ProtocolResponse &response) const
{
    sendMsg(client, JsonResponsePacketSerializer::serializeResponse(response));
}

RequestInfo CommonCommunicator::_waitForClientRequest(const SOCKET socket)
{
    unsigned char reqCode;
    receiveMsg(socket, &reqCode, SIZE_CODE);

    int jsonLen;
    receiveMsg(socket, &jsonLen, SIZE_JSON_LEN);
    jsonLen = ntohl(jsonLen) * sizeof(char);

    // We do this check here too to validify whether the json MAY be read.
    // This is NOT a part of parsing.
    if (jsonLen <= 0)
    {
        throw std::runtime_error("Invalid JSON length: Reading phase");
    }

    const RequestInfo* info = nullptr;

    unsigned char* const data = new unsigned char[jsonLen]; // readJson already handles null termination.

    try
    {
        receiveMsg(socket, data, jsonLen);

        std::lock_guard lock(this->m_clientsMutex);

        info = new RequestInfo(
            *this->m_clients.at(socket),

            static_cast<RequestCode>(reqCode),
            std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::now()
            ),
            JsonRequestPacketDeserializer::readJson(data, jsonLen)
        );
    }
    catch (const std::exception &)
    {
        delete[] data;
        throw;
    }

    delete[] data;

    RequestInfo result = *info;
    delete info;

    return result;
}

void CommonCommunicator::_clientCleanerThreadFunc()
{
	std::unique_lock lock(this->_disconnectedClientCV_mutex);
	
	while (this->_running)
	{
	    if (this->_disconnectingClients.empty())
	    {
		    // Wait for _enqueueDisconnectClient to be called
		    this->_disconnectedClientConditionalVariable.wait(lock);
	    }

		_freeDisconnectedClients();
	}
}

void CommonCommunicator::_enqueueDisconnectClient(const SOCKET socket)
{	
	std::cout << "Socket " << std::to_string(socket) << " disconnected" << std::endl;

	this->_disconnectingClients_mutex.lock();
	this->_disconnectingClients.push_back(socket);
	this->_disconnectingClients_mutex.unlock();
	
	this->_disconnectedClientConditionalVariable.notify_one();
}

void CommonCommunicator::_freeDisconnectedClients()
{
    std::unique_lock disconnectingClientsLock(this->_disconnectingClients_mutex);
    std::unique_lock clientsLock(this->m_clientsMutex);

	for (const auto& clientSock : this->_disconnectingClients)
	{
		delete this->m_clients.at(clientSock);
		m_clients.erase(clientSock);
	}

	clientsLock.unlock();
	
	this->_disconnectingClients.clear();
	disconnectingClientsLock.unlock();
}
