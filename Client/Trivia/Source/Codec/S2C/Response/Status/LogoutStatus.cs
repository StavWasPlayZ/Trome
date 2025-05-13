namespace Trivia.Codec.S2C.Response.Status;

public enum LogoutStatus : uint
{
    Success = 1,
    FailedNotLoggedIn = 2,
    FailedInternalError = 0
}