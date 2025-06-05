using Trivia.Models;

namespace Trivia.Codec.S2C.Response.Packets.Impl;

public interface IQuestionResponse : IProtocolResponse
{
    QuestionModel? Question { get; init; }
    int Points { get; init; }
}
