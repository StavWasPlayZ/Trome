#pragma once
#include "IRequestHandler.h"

struct GetRoomStateRequest;
class RequestHandlerFactory;

class RoomRequestHandler : public IRequestHandler
{
public:
    explicit RoomRequestHandler(const RequestHandlerFactory &handlerFactory, Room &room);

    bool isRequestRelevant(const RequestInfo &info) const override;
    RequestResult handleRequest(const RequestInfo &info, const ProtocolRequest &request) const override;

protected:
    Room &m_room;

private:
    RequestResult getRoomState(const RequestInfo &info, const GetRoomStateRequest &request) const;
};
