namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct GetRoomsRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetRooms;
}