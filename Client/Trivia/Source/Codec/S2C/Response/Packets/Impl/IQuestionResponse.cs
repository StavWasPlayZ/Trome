using System.Collections.Immutable;
using Trivia.Codec.S2C.Objects;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response.Packets.Impl;

public interface IQuestionResponse : IProtocolResponse
{
    QuestionModel? Question { get; init; }
    int Points { get; init; }
    ImmutableList<PlayerResult>? Results { get; init; }
}
