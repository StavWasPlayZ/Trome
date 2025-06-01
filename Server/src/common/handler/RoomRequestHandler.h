#pragma once
#include "IRequestHandler.h"

class RequestHandlerFactory;

class RoomRequestHandler : public IRequestHandler
{
public:
    explicit RoomRequestHandler(const RequestHandlerFactory &handlerFactory, Room &room);

protected:
    Room &m_room;
};
