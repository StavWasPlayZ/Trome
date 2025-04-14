#pragma once

#include <exception>

class SocketDisconnectionException : public std::exception
{
public:
    explicit SocketDisconnectionException() = default;
    const char* what() const noexcept override;
};
