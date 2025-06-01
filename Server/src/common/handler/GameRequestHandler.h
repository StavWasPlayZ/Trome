#pragma once

#include "IRequestHandler.h"
#include "codec/c2s/request/Request.h"

class Game;
class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
public:
    GameRequestHandler(const RequestHandlerFactory& handlerFactory, Game& game);

    bool isRequestRelevant(const RequestInfo& info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult submitAnswer(const RequestInfo &info, const SubmitAnswerRequest &request) const;
    RequestResult leaveGame(const RequestInfo &info, const LeaveGameRequest &request) const;

    /**
     * Invalidates the current, active (or inactive) question, replacing it with the next one.
     */
    RequestResult getQuestion(const RequestInfo &info, const GetQuestionRequest &request) const;

    RequestResult getGameResults(const RequestInfo &info, const GetGameResultRequest &request) const;


    void handleLastPlayerFinished(const RequestInfo &info) const;

    //NOTE: Could be internal method of room if used more than here.
    IRequestHandler* getMenuRequestHandlerFor(const LoggedUser &user) const;


    Game& m_game;
};
