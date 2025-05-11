namespace Trivia.Codec.S2C.Response.Status;

public enum ErrorStatus : uint
{
    Generic = 0,
    ServerUnimplemented,
    IllegalRequest
}