namespace Trivia.Models;

public record RoomData(
    string Name,
    int MaxPlayers,
    int TimePerQuestionSecs,
    int QuestionsCount
);
