using System.Collections.Generic;

namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct AddQuestionRequest(
    string Question,
    IList<string> Answers
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.AddQuestion;
}