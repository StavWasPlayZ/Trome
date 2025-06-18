namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct JoinRoomRequest(
    long RoomId
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.JoinRoom;
}