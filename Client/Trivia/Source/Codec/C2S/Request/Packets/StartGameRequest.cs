namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct StartGameRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.StartGame;
}