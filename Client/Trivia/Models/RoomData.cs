namespace Trivia.Models;

public record RoomData(
    string Name,
    int MaxPlayers,
    int TimePerQuestion,
    int PlayersCount,
    int QuestionsCount
);
