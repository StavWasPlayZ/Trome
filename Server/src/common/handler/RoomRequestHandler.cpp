#include "RoomRequestHandler.h"

RoomRequestHandler::RoomRequestHandler(const RequestHandlerFactory &handlerFactory, Room &room) :
    IRequestHandler(handlerFactory),
    m_room(room)
{}