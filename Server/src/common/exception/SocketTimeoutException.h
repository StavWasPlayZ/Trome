#pragma once

#include <exception>
#include <string>

class SocketTimeoutException : public std::exception
{
public:
    explicit SocketTimeoutException() = default;
    const char* what() const noexcept override;
};
