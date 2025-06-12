#include "RequestInfo.h"

#include "Utils.h"

RequestInfo::RequestInfo(Client& client, const RequestCode id, const nlohmann::json &data) :
    client(client),
    id(id),
    receivalTime(utils::getCurrTimeMillis()),
    data(data)
{}
