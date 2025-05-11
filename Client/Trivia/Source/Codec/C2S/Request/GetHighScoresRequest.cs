namespace Trivia.Codec.C2S.Request;

public record GetHighScoresRequest()
    : ProtocolRequest(RequestCode.GetHighScores);