using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record JoinRoomResponse(
    ConsumingResponseStatus Status,
    string Name,
    int MaxPlayers,
    int QuestionCount,
    int SecsPerQuestion,
    RoomStatus RoomStatus
) : ProtocolResponse<ConsumingResponseStatus>(Status);