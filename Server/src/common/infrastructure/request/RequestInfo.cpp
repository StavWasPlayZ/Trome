#include "RequestInfo.h"

RequestInfo::RequestInfo(const Client& client, const RequestCode id, const std::time_t receivalTime, const nlohmann::json &data) :
    client(client),
    id(id),
    receivalTime(receivalTime),
    data(data)
{}
