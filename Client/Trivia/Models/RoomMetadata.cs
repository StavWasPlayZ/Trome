namespace Trivia.Models;

public record RoomMetadata(
    string Name,
    int MaxPlayers,
    int TimePerQuestion,
    int PlayersCount,
    int QuestionsCount
);
