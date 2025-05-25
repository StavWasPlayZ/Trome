using Trivia.Codec.S2C.Objects;

namespace Trivia.Codec.S2C.Response.Packets;

public record GetQuestionResponse(
    string Question,
    string[] Answers,
    int Rotation
) : ProtocolResponse;