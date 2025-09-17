#pragma once

#include <exception>

class SocketTimeoutException : public std::exception
{
public:
    explicit SocketTimeoutException();
    const char* what() const noexcept override;
};
