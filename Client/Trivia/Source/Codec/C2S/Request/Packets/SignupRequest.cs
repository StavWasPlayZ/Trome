namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct SignupRequest(
    string Username,
    string Password,
    string Email,
    string Phone,
    string? Address,
    string Birthdate
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.Signup;
}