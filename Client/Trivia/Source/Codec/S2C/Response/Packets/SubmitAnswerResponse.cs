using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct SubmitAnswerResponse(
    Question? NewQuestion,
    bool WasLastPlayer
) : IProtocolResponse;