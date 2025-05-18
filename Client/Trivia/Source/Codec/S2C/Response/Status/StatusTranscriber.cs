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
        
        [SignupStatus.FailedInvalidArgument] = response =>
            $"Invalid {(response as SignupResponse).Context} format"
    };

    public static string Transcribe<TStatus>(ProtocolResponse<TStatus> response)
    {
        return Transcriptions[response.Status](response).ToUpper();
    }
}