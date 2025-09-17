namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct GetHighScoresRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetHighScores;
}