namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct GetQuestionRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetQuestion;
}