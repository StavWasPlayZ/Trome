namespace Trivia.Codec.S2C.Response.Packets.Impl;

public interface IRegistrationResponse : IProtocolResponse
{
    long UserId { get; init; }
}
