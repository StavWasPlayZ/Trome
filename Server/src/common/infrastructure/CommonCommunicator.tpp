#include "CommonCommunicator.h"

#include <iostream>

#include <thread>
#include <chrono>

#include "Constants.h"

#include "exception/ForcedDisconnectionException.h"
#include "exception/SocketTimeoutException.h"

#include "handler/codec/s2c/Response.h"

#include "handler/codec/c2s/JsonRequestPacketDeserializer.h"
#include "handler/codec/s2c/JsonResponsePacketSerializer.h"

#include "handler/LoginRequestHandler.h"


template <typename T>
CommonCommunicator<T>::CommonCommunicator(const T defaultSocket) :
    m_serverSocket(defaultSocket),
    _serverSockAddr({ 0 }),
    _running(false)
{}

template <typename T>
CommonCommunicator<T>::~CommonCommunicator()
{
    close();
}

template <typename T>
bool CommonCommunicator<T>::isRunning() const
{
    return this->_running;
}

template <typename T>
void CommonCommunicator<T>::bindAndListen()
{
    commonSetup();
    startServerThreads();
}

template <typename T>
void CommonCommunicator<T>::close()
{
    if (!this->_running)
        return;

    // Notify all threads that the server is closing
    this->_running = false;

    // Release the client cleaner thread
    this->_disconectedClientConditionalVariable.notify_all();

    // Wait for 'em to close
    this->m_clients_mutex.lock();
    for (const auto& client : this->m_clients)
    {
        client.second->thread.wait();
    }
    this->m_clients_mutex.unlock();

    platformClose();

    this->_serverSockAddr = { 0 };
    this->_serverThread = std::future<void>();
}

template <typename T>
void CommonCommunicator<T>::commonSetup()
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
        bind(this->m_serverSocket, (struct sockaddr*)&this->_serverSockAddr, sizeof(this->_serverSockAddr))
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

template <typename T>
void CommonCommunicator<T>::registerClient(const T socket)
{
    this->m_clients[socket] = new Client<T>(
        socket,
        new LoginRequestHandler(),
        [this, socket]()
        {
            _clientThreadFunc(socket);
        }
    );

    std::cout << "Connection accepted from " + std::to_string(socket) << std::endl;
}

template <typename T>
void CommonCommunicator<T>::startServerThreads()
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

template <typename T>
void CommonCommunicator<T>::sendMsg(const T socket, const unsigned char* buffer, const int length) const
{
    bool didError;

    try
    {
        didError = send(socket, buffer, length, 0) == -1;
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

template <typename T>
void CommonCommunicator<T>::throwPlatformError(const std::string& msg) const
{
    throw std::runtime_error(msg);
}

template <typename T>
void CommonCommunicator<T>::_serverThreadFunc()
{
    while (this->_running)
    {
        acceptClients();
    }

    close();
}

template <typename T>
void CommonCommunicator<T>::_clientThreadFunc(const T socket)
{
    while (this->_running)
    {
        try
        {
            _handleClient(socket);
        }
        catch (const SocketTimeoutException& e)
        {
            // If we timed out (see RECV_REFRESH_TIMEOUT),
            // simply wait for the next recv cycle (if applicable).
            continue;
        }
        catch (const ForcedDisconnectionException& e)
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
template <typename T>
void CommonCommunicator<T>::_handleClient(const T socket)
{
    const RequestInfo info = _waitForClientRequest(socket);

    Client<T>* client = this->m_clients.at(socket);
    const IRequestHandler* const handler = client->requestHandler;

    OBuffer responseBuffer;

    if (!handler->isRequestRelevant(info))
    {
        responseBuffer = JsonResponsePacketSerializer::serializeResponse(
            ErrorResponse("Illegal request")
        );
    }
    else
    {
        const RequestResult result = handler->handleRequest(info);

        // The Handler did its job well.
        // 🫡
        delete handler;

        responseBuffer = result.response;
        client->requestHandler = result.newHandler;
    }

    sendMsg(socket, responseBuffer.contents, responseBuffer.length);
    responseBuffer.freeContents();
}

template <typename T>
RequestInfo CommonCommunicator<T>::_waitForClientRequest(const T socket)
{
    unsigned char reqCode;
    recieveMsg(socket, &reqCode, SIZE_CODE);

    int jsonLen;
    recieveMsg(socket, &jsonLen, SIZE_JSON_LEN);
    jsonLen = ntohl(jsonLen) * sizeof(char);

    if (jsonLen <= 0)
    {
        throw std::runtime_error("Invalid JSON length");
    }

    unsigned char* const data = new unsigned char[jsonLen]; // readJson already handles null termination.
    recieveMsg(socket, data, jsonLen);

    const RequestInfo info(
        (ProtocolCode)reqCode,
        std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        ),
        JsonRequestPacketDeserializer::readJson(data, jsonLen)
    );

    delete[] data;

    return info;
}

template <typename T>
void CommonCommunicator<T>::_clientCleanerThreadFunc()
{
	std::unique_lock<std::mutex> lock(this->_disconectedClient_mutex);
	
	while (this->_running)
	{
		// waits for _enqueueDisconnectClient to be called
		this->_disconectedClientConditionalVariable.wait(lock);

		_freeDisconnectedClients();
	}
}

template <typename T>
void CommonCommunicator<T>::_enqueueDisconnectClient(const T socket)
{	
	std::cout << "Socket " << std::to_string(socket) << " disconected" << std::endl;

	this->_disconnectingClients_mutex.lock();
	this->_disconnectingClients.push_back(socket);
	this->_disconnectingClients_mutex.unlock();
	
	this->_disconectedClientConditionalVariable.notify_one();
}

template <typename T>
void CommonCommunicator<T>::_freeDisconnectedClients()
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
