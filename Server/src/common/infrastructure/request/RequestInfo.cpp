#include "RequestInfo.h"

RequestInfo::RequestInfo(const ProtocolCode id, const std::time_t receivalTime, const nlohmann::json &data) :
    id(id),
    receivalTime(receivalTime),
    data(data)
{}
