namespace Trivia.Codec.S2C.Response;

public enum ErrorStatus
{
    // Generic
    Internal,
    ServerUnimplemented,
    IllegalRequest,

    /// The status of a response that takes in a resource.
    ///
    /// May be of any type.
    ///
    /// E.g: User not found.
    ErrorUnknownResource,

    // Login
    FailedInvalidCredentials,
    FailedAlreadyLoggedIn,

    // Signup
    FailedUsernameTaken,
    FailedInvalidArgument,

    // Logout
    FailedNotLoggedIn
}