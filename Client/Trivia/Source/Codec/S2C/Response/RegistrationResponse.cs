namespace Trivia.Codec.S2C.Response;

public abstract record RegistrationResponse(
    int UserId
) : ProtocolResponse;