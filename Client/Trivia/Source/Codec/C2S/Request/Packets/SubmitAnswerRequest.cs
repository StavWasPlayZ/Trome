namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct SubmitAnswerRequest(
    int Answer
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.SubmitAnswer;
}