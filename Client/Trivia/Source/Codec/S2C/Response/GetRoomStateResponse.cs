using Trivia.Codec.S2C.Response;
using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Source.Codec.S2C.Response;

// TODO: Remove this when merging with branch 37
public enum RoomStatus : uint
{
    Waiting = 0,
    Playing = 1,
    NotFound = 2
}


public record GetRoomStateResponse(
        GenericResponseStatus Status,
        RoomStatus GameStatus,
        int QuestionCount,
        int SecsPerQuestion,
        int Players,
        bool IsGameRunning
    ) : ProtocolResponse<GenericResponseStatus>(Status);