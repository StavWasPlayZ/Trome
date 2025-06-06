using System.Collections.Immutable;
using Trivia.Codec.S2C.Response.Packets.Impl;
using Trivia.Models.User;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct GetHighScoresResponse(
    ImmutableList<UserScoreModel> HighScores
) : IProtocolResponse;
