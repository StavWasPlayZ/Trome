namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct LoginRequest(
    string Username,
    string Password
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.Login;
}