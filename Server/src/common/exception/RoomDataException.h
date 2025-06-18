#pragma once

#include <exception>
#include <string>

class RoomDataException : public std::exception
{
public:
	explicit RoomDataException(const std::string &err);
	const char *what() const noexcept override;

private:
	const std::string message;
};