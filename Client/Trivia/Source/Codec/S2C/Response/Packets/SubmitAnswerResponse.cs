using Trivia.Codec.S2C.Response.Packets.Impl;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct SubmitAnswerResponse(
    QuestionModel? Question,
    int Points
) : IQuestionResponse;
