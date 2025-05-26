namespace Trivia.Codec.C2S.Request.Packets;

public record GetQuestionRequest() : ProtocolRequest(RequestCode.GetQuestion);