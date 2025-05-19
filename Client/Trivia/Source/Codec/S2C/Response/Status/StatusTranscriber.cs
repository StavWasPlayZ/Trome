using System;
using System.Collections.Generic;

namespace Trivia.Codec.S2C.Response.Status;

public static class StatusTranscriber
{
    private static readonly Dictionary<object, Func<IProtocolResponse, string>> Transcriptions = new()
    {
        [1] = _ => "Success",

            
        [LoginStatus.FailedInvalidCredentials] = _ =>
            "Invalid username or password. Please try again.",
        [LoginStatus.FailedAlreadyLoggedIn] = _ =>
            "User is already logged in. Log out of the other game session and try again.",
        [LoginStatus.FailedInternalError] = _ =>
            "The server faulted",
        
        
        [SignupStatus.FailedInvalidArgument] = response =>
            $"Invalid {(response as SignupResponse)!.Context} format",
        [SignupStatus.FailedUsernameTaken] = _ =>
            "Username Taken",
        [SignupStatus.FailedInternalError] = _ =>
            "The server faulted",
        
        
        [ErrorStatus.Generic] = response =>
            (response as ErrorResponse)!.Message,
        [ErrorStatus.IllegalRequest] = response =>
            $"Illegal request: {(response as ErrorResponse)!.Message}",
        [ErrorStatus.ServerUnimplemented] = response =>
            $"Unimplemented server request used: {(response as ErrorResponse)!.Message}",
        
        
        [LogoutStatus.FailedNotLoggedIn] = _ =>
            "The user isn't logged in",
        [LogoutStatus.FailedInternalError] = _ =>
            "The server faulted",
        
        
        [GenericResponseStatus.Error] = _ =>
            "The server faulted",
        
        
        [GenericStatsStatus.Error] = _ =>
            "The server faulted",
        
        
        [ConsumingResponseStatus.ErrorInternal] = _ =>
            "The server faulted",
        [ConsumingResponseStatus.ErrorUnknownResource] = _ =>
            "The requested resource could not be found"
    };

    public static string Transcribe<TStatus>(ProtocolResponse<TStatus> response)
    {
        return (
            Transcriptions.TryGetValue(response.Status!, out var transcriber)
                ? transcriber(response)
                : "Untranscribable error occured"
        ).ToUpper();
    }
}