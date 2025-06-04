#include "GameManager.h"

GameManager::GameManager(const IDatabase& database, RoomManager& roomManager) :
    m_database(database),
    m_roomManager(roomManager)
{}

Game& GameManager::createGame(Room &room)
{
    const auto [result, _] = this->m_games.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(room.getId()),
        std::forward_as_tuple(room, m_database, m_roomManager)
    );

    return result->second;
}

Game &GameManager::getGameByRoom(const Room &room)
{
    const std::optional<Game*> game = room.getCurrentGame();

    if (!game.has_value())
        throw std::runtime_error("Game not found");

    return *game.value();
}

void GameManager::deleteGame(const Game &game)
{
    game.getRoom().unsetCurrentGame();
    this->m_games.erase(game.getId());
}

ProtocolResponse *GameManager::addQuestion(const RequestInfo &info, const Question &question, const LoggedUser& user)
{
    // addQuestions get vector of questions therefore:
    std::vector<Question> questions;
    questions.push_back(question);

    try
    {
        this->m_database.addQuestions(questions, user.getUsername());
    }
    catch (const std::runtime_error &e)
    {
        // addQuestions will return runtime_error when adding a user with the same question because it's UNIQUE.
        // Note that the full message reads as follows:
        // "Error in SQL: UNIQUE constraint failed: questions.question"

        if (std::strstr(e.what(), "UNIQUE") != nullptr)
        {
            return new ErrorResponse(ErrorStatus::QUESTION_ALREADY_EXISTS, info.id);
        }

        return new ErrorResponse(ErrorStatus::INVALID_ARGUMENT, info.id, e.what());
    }

    return new AddQuestionResponse();
}
