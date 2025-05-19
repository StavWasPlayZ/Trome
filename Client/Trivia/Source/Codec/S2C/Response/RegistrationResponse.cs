namespace Trivia.Codec.S2C.Response;

/// <typeparam name="TStatus">The enum status type</typeparam>
public abstract record RegistrationResponse<TStatus>(
    TStatus Status,
    int? UserId
) : ProtocolResponse<TStatus>(Status);