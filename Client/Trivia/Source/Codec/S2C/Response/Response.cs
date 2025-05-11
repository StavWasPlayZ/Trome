using System.Collections.Generic;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public enum ResponseCode : byte
{
    Error = 0,
    Login,
    Signup,
    Logout,
    JoinRoom,
    CreateRoom,
    GetRooms,
    GetPlayerInRoom,
    GetHighScores,
    GetPersonalStatistics
}

public enum GenericResponseStatus : uint
{
    Success = 1,
    Error = 0
}

public enum ConsumingResponseStatus  : uint
{
    Success = 1,
    ErrorUnknownResource,
    ErrorInternal = 0
}

/// <typeparam name="TStatus">The enum status type</typeparam>
public abstract record ProtocolResponse<TStatus>(
    TStatus Status
);

/// <typeparam name="TStatus">The enum status type</typeparam>
public abstract record RegistrationResponse<TStatus>(
    TStatus Status,
    int? UserId
) : ProtocolResponse<TStatus>(Status);

public enum LoginStatus : uint
{
    Success = 1,
    FailedInvalidCredentials = 2,
    FailedAlreadyLoggedIn = 3,
    FailedInternalError = 0
}

public record LoginResponse(
    LoginStatus Status,
    int? UserId
) : RegistrationResponse<LoginStatus>(Status, UserId);

public enum SignupStatus : uint
{
    Success = 1,
    FailedUsernameTaken = 2,
    FailedInvalidArgument = 3,
    FailedInternalError = 0
}

public record SignupResponse(
    SignupStatus Status,
    int? UserId = null
) : RegistrationResponse<SignupStatus>(Status, UserId);

public enum LogoutStatus : uint
{
    Success = 1,
    FailedNotLoggedIn = 2,
    FailedInternalError = 0
}

public record LogoutResponse(
    LogoutStatus Status
) : ProtocolResponse<LogoutStatus>(Status);

public enum ErrorStatus : uint
{
    Generic = 0,
    ServerUnimplemented,
    IllegalRequest
}

public record ErrorResponse(
    ErrorStatus Status,
    string Message
) : ProtocolResponse<ErrorStatus>(Status);

//TODO: Add rooms metadata
public record JoinRoomResponse(
    ConsumingResponseStatus Status
) : ProtocolResponse<ConsumingResponseStatus>(Status);

public record CreateRoomResponse(
    GenericResponseStatus Status,
    int RoomId
) : ProtocolResponse<GenericResponseStatus>(Status);


//TODO: When Room is implemented, add below to both.
public record GetRoomsResponse(
    GenericResponseStatus Status
) : ProtocolResponse<GenericResponseStatus>(Status);

public record GetPlayersInRoomResponse(
    GenericResponseStatus Status
) : ProtocolResponse<GenericResponseStatus>(Status);


public enum GenericStatsStatus : uint
{
    Success = 1,
    Error = 0,
    NotConnectedError = 2
}

public record GetHighScoresResponse(
    GenericStatsStatus Status,
    IReadOnlyList<KeyValuePair<string, int>> Stats
) : ProtocolResponse<GenericStatsStatus>(Status);

public record GetPersonalStatisticsResponse(
    GenericStatsStatus Status,
    UserStatistics Stats
) : ProtocolResponse<GenericStatsStatus>(Status);
