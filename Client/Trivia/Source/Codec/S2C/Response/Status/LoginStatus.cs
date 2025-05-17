namespace Trivia.Codec.S2C.Response.Status;

public enum LoginStatus : uint
{
    Success = 1,
    FailedInvalidCredentials = 2,
    FailedAlreadyLoggedIn = 3,
    FailedInternalError = 0
}