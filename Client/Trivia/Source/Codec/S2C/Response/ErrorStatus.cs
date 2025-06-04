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
    UnknownResource,

    // Login
    InvalidCredentials,
    AlreadyLoggedIn,

    // Signup
    UsernameTaken,
    InvalidArgument,

    // Logout
    NotLoggedIn,
    
    // Room
    RoomFull,
    RoomAlreadyPlaying,
    
    // Game
    QuestionOutdated,
    AlreadyFinished,
    
    // Adding Question
    QuestionAlreadyExists
}