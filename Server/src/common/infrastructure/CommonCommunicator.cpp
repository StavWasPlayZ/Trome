#include "CommonCommunicator.h"

#include <iostream>

#include <thread>

#include "Constants.h"

#include "exception/SocketDisconnectionException.h"
#include "exception/SocketTimeoutException.h"

#include "handler/codec/s2c/Response.h"

#include "handler/codec/c2s/JsonRequestPacketDeserializer.h"
#include "handler/codec/s2c/JsonResponsePacketSerializer.h"


CommonCommunicator::CommonCommunicator(const SOCKET defaultSocket, const RequestHandlerFactory& handlerFactory) :
    _running(false),
    _serverSockAddr({}),
    m_serverSocket(defaultSocket),
    m_handlerFactory(handlerFactory)
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
    this->m_clients_mutex.lock();
    for (const auto& client : this->m_clients)
    {
        client.second->thread.wait();
    }
    this->m_clients_mutex.unlock();

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
    setRecvTimeout(RECV_REFRESH_TIMEOUT_MS);

    // Set server address information
    this->_serverSockAddr.sin_family = AF_INET;
    this->_serverSockAddr.sin_addr.s_addr = INADDR_ANY;
    this->_serverSockAddr.sin_port = htons(PORT);

    if (!isValidBind(
        bind(this->m_serverSocket, (sockaddr*)&this->_serverSockAddr, sizeof(this->_serverSockAddr))
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
    this->m_clients[socket] = new Client(
        socket,
        this->m_handlerFactory.createLoginRequestHandler(),
        [this, socket]()
        {
            _clientThreadFunc(socket);
        }
    );

    std::cout << "Connection accepted from " + std::to_string(socket) << std::endl;
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

void CommonCommunicator::sendMsg(const SOCKET socket, const unsigned char* buffer, const int length) const
{
    bool didError;

    try
    {
        // Casting for crybaby Windows
        didError = send(socket, (char*)buffer, length, 0) == -1;
    }
    catch (...)
    {
        didError = true;
    }

    if (didError)
    {
        throwPlatformError("Failed to send message to client socket " + std::to_string(socket));
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

    _enqueueDisconnectClient(socket);
}

//ANCHOR Actual client processing function.
void CommonCommunicator::_handleClient(const SOCKET socket) const
{
    const RequestInfo info = _waitForClientRequest(socket);

    Client* const client = this->m_clients.at(socket);
    const IRequestHandler* const handler = client->requestHandler;

    OBuffer responseBuffer;

    if (!handler->isRequestRelevant(info))
    {
        responseBuffer = JsonResponsePacketSerializer::serializeResponse(
            ErrorResponse(ErrorStatus::ILLEGAL_REQUEST, "Illegal request")
        );
    }
    else
    {
        const ProtocolRequest* request = ProtocolRequest::fromRequest(info);
        const RequestResult result = handler->handleRequest(info, *request);
        delete request;

        // The Handler did its job well.
        // 🫡
        delete handler;

        responseBuffer = result.response;
        client->requestHandler = result.newHandler;
    }

    sendMsg(socket, responseBuffer.contents, responseBuffer.length);
    responseBuffer.freeContents();
}

RequestInfo CommonCommunicator::_waitForClientRequest(const SOCKET socket) const
{
    unsigned char reqCode;
    receiveMsg(socket, &reqCode, SIZE_CODE);

    int jsonLen;
    receiveMsg(socket, &jsonLen, SIZE_JSON_LEN);
    jsonLen = ntohl(jsonLen) * sizeof(char);

    if (jsonLen <= 0)
    {
        throw std::runtime_error("Invalid JSON length");
    }

    unsigned char* const data = new unsigned char[jsonLen]; // readJson already handles null termination.
    receiveMsg(socket, data, jsonLen);

    const RequestInfo info(
        *this->m_clients.at(socket),

        (ProtocolCode)reqCode,
        std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        ),
        JsonRequestPacketDeserializer::readJson(data, jsonLen)
    );

    delete[] data;

    return info;
}

void CommonCommunicator::_clientCleanerThreadFunc()
{
	std::unique_lock lock(this->_disconnectedClient_mutex);
	
	while (this->_running)
	{
		// Waits for _enqueueDisconnectClient to be called
		this->_disconnectedClientConditionalVariable.wait(lock);

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
	this->_disconnectingClients_mutex.lock();
	this->m_clients_mutex.lock();

	for (const auto& clientSock : this->_disconnectingClients)
	{
		delete this->m_clients.at(clientSock);
		m_clients.erase(clientSock);
	}

	this->m_clients_mutex.unlock();
	
	this->_disconnectingClients.clear();
	this->_disconnectingClients_mutex.unlock();
}
