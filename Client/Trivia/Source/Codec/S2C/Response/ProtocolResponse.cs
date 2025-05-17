namespace Trivia.Codec.S2C.Response;

/// <typeparam name="TStatus">The enum status type</typeparam>
public abstract record ProtocolResponse<TStatus>(
    TStatus Status
) : IProtocolResponse;

public interface IProtocolResponse;
