namespace Trivia.Codec.C2S.Request.Packets;

public record GetGameResultRequest() : ProtocolRequest(RequestCode.GetGameResult);