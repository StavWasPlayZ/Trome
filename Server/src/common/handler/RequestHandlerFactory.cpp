#include "RequestHandlerFactory.h"

#include "MenuRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(
    LoginManager& loginManager,
    RoomManager& roomManager,
    StatisticsManager& statisticsManager,
    GameManager& gameManager,
    const IDatabase& database
) :
    m_loginManager(loginManager),
    m_roomManager(roomManager),
    m_statisticsManager(statisticsManager),
    m_gameManager(gameManager),
    m_database(database)
{}

LoginManager &RequestHandlerFactory::getLoginManager() const
{
    return this->m_loginManager;
}
RoomManager &RequestHandlerFactory::getRoomManager() const
{
    return this->m_roomManager;
}
StatisticsManager &RequestHandlerFactory::getStatisticsManager() const
{
    return this->m_statisticsManager;
}

GameManager &RequestHandlerFactory::getGameManager() const
{
    return this->m_gameManager;
}

const LoginRequestHandler *RequestHandlerFactory::createLoginRequestHandler() const
{
    return new LoginRequestHandler(*this);
}
const MenuRequestHandler *RequestHandlerFactory::createMenuRequestHandler() const
{
    return new MenuRequestHandler(*this);
}
