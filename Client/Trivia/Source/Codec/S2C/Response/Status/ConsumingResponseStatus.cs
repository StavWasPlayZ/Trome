namespace Trivia.Codec.S2C.Response.Status;

public enum ConsumingResponseStatus  : uint
{
    Success = 1,
    ErrorUnknownResource,
    ErrorInternal = 0
}