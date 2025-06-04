namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct AddQuestionRequest(
    string Question,
    string Correct,
    string Wrong1,
    string Wrong2,
    string Wrong3
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.AddQuestion;
}