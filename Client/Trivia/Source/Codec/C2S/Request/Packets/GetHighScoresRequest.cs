namespace Trivia.Codec.C2S.Request.Packets;

public record GetHighScoresRequest()
    : ProtocolRequest(RequestCode.GetHighScores);