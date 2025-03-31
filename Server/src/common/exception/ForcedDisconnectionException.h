#pragma once

#include <exception>
#include <string>

class ForcedDisconnectionException : public std::exception
{
public:
    explicit ForcedDisconnectionException() = default;
    const char* what() const noexcept override;
};
