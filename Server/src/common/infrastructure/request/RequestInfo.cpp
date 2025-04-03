#include "RequestInfo.h"

RequestInfo::RequestInfo(const ProtocolCode id, const time_t receivalTime, const unsigned char *const buffer) :
    id(id),
    receivalTime(receivalTime),
    buffer(buffer)
{}
