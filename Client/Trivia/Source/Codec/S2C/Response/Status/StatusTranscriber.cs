using System;
using System.Collections.Generic;

namespace Trivia.Codec.S2C.Response.Status;

public static class StatusTranscriber
{
    private static readonly Dictionary<object, string> transcribtions = new()
    {
        [1] = "Success",

        [LoginStatus.FailedInvalidCredentials] = "Invalid username or password. Please try again."
    };

    public static string Transcribe<TStatus>(ProtocolResponse<TStatus> response)
    {
        return transcribtions[response.Status];
    }
}