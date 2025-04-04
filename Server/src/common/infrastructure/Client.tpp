#pragma once

#include "Client.h"

template <typename T>
inline Client<T>::Client(const T socket, const IRequestHandler *const requestHandler, const std::function<void()> clientThreadFunc) :
    socket(socket),
    requestHandler(requestHandler),
    thread(std::async(std::launch::async, clientThreadFunc))
{}

template <typename T>
Client<T>::~Client()
{
    delete this->requestHandler;
}
