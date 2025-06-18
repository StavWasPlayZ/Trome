#include "RoomDataException.h"

RoomDataException::RoomDataException(const std::string &err) : message(err)
{
}

const char *RoomDataException::what() const noexcept
{
    return message.c_str();
}
