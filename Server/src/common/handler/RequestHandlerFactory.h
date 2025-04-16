#pragma once

#include "infrastructure/db/IDatabase.h"
#include "manager/LoginManager.h"

#include "handler/LoginRequestHandler.h"

// Note that this class is a bit useless, as we can just supply these
// arguments to the various handlers by ourselves using the
// already existing IRequestHandler.
// 
// But oh well.
class RequestHandlerFactory
{
public:
    RequestHandlerFactory(LoginManager& loginManager, const IDatabase& database);

    LoginManager& getLoginManager() const;

    /**
     * NOTE: The returned resource must be freed.
     */
    const LoginRequestHandler* createLoginRequestHandler() const;

private:
    //NOTE: Actual LoginManager instantiated via Server (or otherwise).
    LoginManager& loginManager;
    const IDatabase& m_database;
};
