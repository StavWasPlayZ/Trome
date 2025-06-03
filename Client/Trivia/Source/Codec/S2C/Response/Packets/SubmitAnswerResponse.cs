using Trivia.Models;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct SubmitAnswerResponse(
    QuestionModel? NewQuestion,
    int Points
) : IProtocolResponse;