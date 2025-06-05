namespace Trivia.Codec.S2C.Response.Packets.Impl;

public interface IRegistrationResponse : IProtocolResponse
{
    int UserId { get; init; }
}
