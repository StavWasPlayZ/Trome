namespace Trivia.Codec.S2C.Response;

public interface IRegistrationResponse : IProtocolResponse
{
    int UserId { get; init; }
}