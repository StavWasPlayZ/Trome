namespace Trivia.Codec.C2S.Request.Packets;

// TODO: Make this submit the actual answer
public record SubmitAnswerRequest() : ProtocolRequest(RequestCode.SubmitAnswer);