#pragma once

#include "IRequestHandler.h"
#include "codec/c2s/request/Request.h"

class Game;
class RequestHandlerFactory;
enum class QuestionRollType;
struct QuestionRollResult;

class GameRequestHandler : public IRequestHandler
{
public:
    GameRequestHandler(const RequestHandlerFactory& handlerFactory, Game& game);

    bool isRequestRelevant(const RequestInfo& info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult submitAnswer(const RequestInfo &info, const SubmitAnswerRequest &request) const;
    RequestResult leaveGame(const RequestInfo &info, const LeaveGameRequest &request) const;

    QuestionRollResult rollNewUserQuestion(const RequestInfo &info, bool didFail) const;

    /**
     * Invalidates the current, active (or inactive) question, replacing it with the next one.
     */
    RequestResult getQuestion(const RequestInfo &info, const GetQuestionRequest &request) const;

    RequestResult getGameResults(const RequestInfo &info, const GetGameResultRequest &request) const;


    std::vector<PlayerResult> handleLastPlayerFinished(const RequestInfo &info) const;


    Game& m_game;
};


enum class QuestionRollType
{
    ROLLED,
    FINISHED,
    FINISHED_LAST
};

struct QuestionRollResult
{
    QuestionRollResult(QuestionRollType rollType, const std::optional<UserQuestion>& newQuestion,
                       const std::optional<std::vector<PlayerResult>>& results = std::nullopt);

    const QuestionRollType rollType;
    const std::optional<UserQuestion> newQuestion;

    /**
     * If the player has finished, and finished last - the results will be provided here.
     */
    const std::optional<std::vector<PlayerResult>> results;
};
