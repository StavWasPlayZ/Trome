namespace Trivia.Models.Raw;

public class RoomData
{
    public required string Name { get; init; }
    public required int MaxPlayers { get; init; }
    public required int TimePerQuestionSecs { get; init; }
    public required int QuestionsCount { get; init; }
}
