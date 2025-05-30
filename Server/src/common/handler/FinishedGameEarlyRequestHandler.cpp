#include "handler/FinishedGameEarlyRequestHandler.h"

FinishedGameEarlyRequestHandler::FinishedGameEarlyRequestHandler(const RequestHandlerFactory &handlerFactory, Game &game)
    : IRequestHandler(handlerFactory), game(game)
{}

bool FinishedGameEarlyRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    return info.id == RequestCode::LEAVE_GAME && !this->m_handlerFactory.getLoginManager().isLoggedIn(info.client);
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
    game.getRoom().removeUser(getUserByInfo(info));

    return RequestResult(new LeaveRoomResponse(), new MenuRequestHandler(m_handlerFactory));
}