namespace Trivia.Codec.S2C.Response.Status;

public enum GenericStatsStatus : uint
{
    Success = 1,
    Error = 0,
    NotConnectedError = 2
}