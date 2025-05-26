using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public record GetQuestionResponse(
    Question Question
) : ProtocolResponse;
