#pragma once

#include <exception>
#include <string>

class WSAException : public std::exception
{
public:
    explicit WSAException(const std::string& message);

    const char* what() const noexcept override;

private:
    std::string _message;
};
