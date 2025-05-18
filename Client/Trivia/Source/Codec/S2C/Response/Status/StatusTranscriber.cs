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
        
        [SignupStatus.FailedInvalidArgument] = response =>
            $"Invalid {(response as SignupResponse)!.Context} format"
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