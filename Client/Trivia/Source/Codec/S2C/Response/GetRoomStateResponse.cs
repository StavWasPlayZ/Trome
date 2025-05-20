using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record GetRoomStateResponse(
    GenericResponseStatus Status,
    RoomStatus GameStatus,
    int QuestionCount,
    int SecsPerQuestion,
    int Players,
    bool IsGameRunning
) : ProtocolResponse<GenericResponseStatus>(Status);