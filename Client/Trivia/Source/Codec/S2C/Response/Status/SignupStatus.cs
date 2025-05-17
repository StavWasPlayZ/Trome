namespace Trivia.Codec.S2C.Response.Status;

public enum SignupStatus : uint
{
    Success = 1,
    FailedUsernameTaken = 2,
    FailedInvalidArgument = 3,
    FailedInternalError = 0
}
