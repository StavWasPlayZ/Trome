namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct LogoutRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.Logout;
}
