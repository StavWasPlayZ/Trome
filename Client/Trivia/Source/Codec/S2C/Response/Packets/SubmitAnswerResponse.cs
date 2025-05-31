using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct SubmitAnswerResponse(
    Question? NewQuestion,
    int Points,
    bool WasLastPlayer,
    int PlayersFinished
) : IProtocolResponse;