#include "handler/FinishedGameEarlyRequestHandler.h"

#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "codec/s2c/response/Response.h"

FinishedGameEarlyRequestHandler::FinishedGameEarlyRequestHandler(const RequestHandlerFactory &handlerFactory, Game &game)
    : IRequestHandler(handlerFactory),
    m_game(game)
{}

bool FinishedGameEarlyRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    return info.id == RequestCode::LEAVE_GAME;
}

RequestResult FinishedGameEarlyRequestHandler::handleRequest(const RequestInfo& info, const ProtocolRequest& request) const
{
    switch (info.id)
    {
    case RequestCode::LEAVE_GAME:
        return leaveGame(info, static_cast<const LeaveGameRequest &>(request));

    default:
        throw std::runtime_error("Unexpected request ID");
    }
}

RequestResult FinishedGameEarlyRequestHandler::leaveGame(const RequestInfo &info, const LeaveGameRequest &) const
{
    Room& room = m_game.getRoom();
    const LoggedUser& user = getUserByInfo(info);

    if (room.getAdmin() == user)
    {
        this->m_handlerFactory.getRoomManager().deleteRoom(room);
    }
    else
    {
        room.removeUser(getUserByInfo(info));
    }

    return RequestResult(new LeaveGameResponse(), new MenuRequestHandler(m_handlerFactory));
}