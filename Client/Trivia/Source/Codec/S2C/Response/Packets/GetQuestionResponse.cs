using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct GetQuestionResponse(
    QuestionModel? Question,
    int Points
) : IProtocolResponse;
