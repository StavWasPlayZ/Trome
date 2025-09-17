using Trivia.Codec.C2S;

namespace Trivia.Models.Raw;

public readonly record struct RoomData(
    string Name,
    int MaxPlayers,
    int TimePerQuestionSecs,
    int QuestionsCount
)
{
    public static RoomData CreateMock(string roomName, RoomType roomType)
    {
        return new RoomData
        {
            Name = roomName,
            MaxPlayers = roomType == RoomType.HeadToHead ? 2 : 10,
            TimePerQuestionSecs = 7,
            QuestionsCount = 15
        };
    }
}
