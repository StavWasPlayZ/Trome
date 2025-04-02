#include "WSAException.h"

#include <string>

#include <Windows.h>

WSAException::WSAException(const std::string& message) :
	_message(message + ": " + std::to_string(WSAGetLastError()))
{}

const char* WSAException::what() const noexcept
{
	return this->_message.c_str();
}
