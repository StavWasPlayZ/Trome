#pragma once

#include "handler/LoginRequestHandler.h"
#include "MenuRequestHandler.h"

#include "infrastructure/db/IDatabase.h"

#include "manager/LoginManager.h"
#include "manager/RoomManager.h"
#include "manager/StatisticsManager.h"

// Note that this class is a bit useless, as we can just supply these
// arguments to the various handlers by ourselves using the
// already existing IRequestHandler.
// 
// But oh well.

class RequestHandlerFactory
{
public:
    RequestHandlerFactory(
        LoginManager& loginManager,
        RoomManager& roomManager,
        StatisticsManager& statisticsManager,
        GameManager& gameManager,
        const IDatabase& database
    );

    LoginManager& getLoginManager() const;
    RoomManager& getRoomManager() const;
    StatisticsManager& getStatisticsManager() const;
    GameManager& getGameManager() const;

    /**
     * NOTE: The returned resource must be freed.
     */
    const LoginRequestHandler* createLoginRequestHandler() const;
    /**
     * NOTE: The returned resource must be freed.
     */
    const MenuRequestHandler* createMenuRequestHandler() const;

private:
    //NOTE: Actual managers instantiated via Server (or otherwise).
    LoginManager& m_loginManager;
    RoomManager& m_roomManager;
    StatisticsManager& m_statisticsManager;
    GameManager &m_gameManager;

    const IDatabase& m_database;
};
