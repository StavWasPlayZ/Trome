using System.Collections.Immutable;
using Trivia.Codec.S2C.Objects;
using Trivia.Codec.S2C.Response.Packets.Impl;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct GetQuestionResponse(
    QuestionModel? Question,
    int Points,
    ImmutableList<PlayerResult>? Results
) : IQuestionResponse;
