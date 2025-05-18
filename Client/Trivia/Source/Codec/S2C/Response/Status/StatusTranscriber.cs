using System;
using System.Collections.Generic;

namespace Trivia.Codec.S2C.Response.Status;

public static class StatusTranscriber
{
    private static readonly Dictionary<object, Func<IProtocolResponse, string>> Transcriptions = new()
    {
        [1] = _ => "Success",

        // login
        [LoginStatus.FailedInvalidCredentials] = _ =>
            "Invalid username or password. Please try again.",
        [LoginStatus.FailedAlreadyLoggedIn] = _ =>
            "User is already logged in. Log out of the other game session and try again.",
        [LoginStatus.FailedInternalError] = _ =>
            "The server faulted",
        
        // signup
        [SignupStatus.FailedInvalidArgument] = response =>
            $"Invalid {(response as SignupResponse)!.Context} format",
        [SignupStatus.FailedUsernameTaken] = _ =>
            "Username Taken",
        [SignupStatus.FailedInternalError] = _ =>
            "The server faulted",
        
        // error
        [ErrorStatus.Generic] = response =>
            $"Error: {(response as ErrorResponse)!.Message}",
        [ErrorStatus.IllegalRequest] = response =>
            $"Error: Illegal request: {(response as ErrorResponse)!.Message}",
        [ErrorStatus.ServerUnimplemented] = _ =>
            "Error: Unimplemented",
        
        // logout
        [LogoutStatus.FailedNotLoggedIn] = _ =>
            "The user isn't logged in",
        [LogoutStatus.FailedInternalError] = _ =>
            "The server faulted",
    };

    public static string Transcribe<TStatus>(ProtocolResponse<TStatus> response)
    {
        return Transcriptions.TryGetValue(response.Status!, out var transcriber)
            ? transcriber(response).ToUpper()
            : "Untranscribable error occured";
    }
}