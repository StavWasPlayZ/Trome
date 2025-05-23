using System;
using Trivia.Codec.S2C.Response.Packets;

namespace Trivia.Exceptions;

public class ServerErrorException(ErrorResponse response) : Exception($"The server returned an exception: {response}")
{
    public ErrorResponse ServerResponse { get; } = response;
}
