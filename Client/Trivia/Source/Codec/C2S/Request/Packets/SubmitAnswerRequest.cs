namespace Trivia.Codec.C2S.Request.Packets;

// TODO: Make this submit the actual answer
public readonly record struct SubmitAnswerRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.SubmitAnswer;
}