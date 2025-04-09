#pragma once

#include <exception>
#include <string>

class SocketDisconnectionException : public std::exception
{
public:
    explicit SocketDisconnectionException() = default;
    const char* what() const noexcept override;
};
