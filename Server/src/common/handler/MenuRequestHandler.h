#pragma once

#include "IRequestHandler.h"

struct JoinRoomRequest;
struct LogoutRequest;
struct GetPersonalStatisticsRequest;
struct GetHighScoresRequest;
struct GetRoomsRequest;
struct CreateRoomRequest;
struct GetPlayersInRoomRequest;

class MenuRequestHandler : public IRequestHandler
{
public:
    explicit MenuRequestHandler(const RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo &info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult joinRoom(const RequestInfo & info, const JoinRoomRequest &request) const;
    RequestResult createRoom(const RequestInfo& info, const CreateRoomRequest &request) const;
    RequestResult getRooms(const RequestInfo& info, const GetRoomsRequest &request) const;
    RequestResult getHighScores(const RequestInfo& info, const GetHighScoresRequest &request) const;
    RequestResult getPersonalStatistics(const RequestInfo& info, const GetPersonalStatisticsRequest &request) const;
    RequestResult logout(const RequestInfo& info, const LogoutRequest &request) const;

    RequestResult getPlayersInRoom(const RequestInfo & info, const GetPlayersInRoomRequest &request) const;
};
