namespace Trivia.Codec.C2S.Request;

public abstract record ProtocolRequest(
    RequestCode Code
);