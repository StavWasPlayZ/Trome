namespace Trivia.Models.Raw;

public readonly record struct RoomData(
    string Name,
    int MaxPlayers,
    int TimePerQuestionSecs,
    int QuestionsCount
);
