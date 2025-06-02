using System;
using System.Collections.Generic;
using Trivia.Codec.S2C.Response.Packets;

namespace Trivia.Codec.S2C.Response;

public static class StatusTranscriber
{
    private static readonly Dictionary<object, Func<ErrorResponse, string>> Transcriptions = new()
    {            
        [ErrorStatus.InvalidCredentials] = _ =>
            "Invalid username or password. Please try again.",
        [ErrorStatus.AlreadyLoggedIn] = _ =>
            "User is already logged in. Log out of the other game session and try again.",
        [ErrorStatus.Internal] = _ =>
            "The server faulted",
        
        
        [ErrorStatus.InvalidArgument] = response =>
            $"Invalid {response.Context} format",
        [ErrorStatus.UsernameTaken] = _ =>
            "Username Taken",
        
        
        [ErrorStatus.IllegalRequest] = response =>
            $"Illegal request: {response.Context}",
        [ErrorStatus.ServerUnimplemented] = response =>
            $"Unimplemented server request used: {response.Context}",
        
        
        [ErrorStatus.NotLoggedIn] = _ =>
            "The user isn't logged in",
        
        [ErrorStatus.UnknownResource] = _ =>
            "The requested resource could not be found",
        
        [ErrorStatus.RoomFull] = _ =>
            "Room is full",
        [ErrorStatus.RoomAlreadyPlaying] = _ =>
            "Room is already playing"
    };

    public static string Transcribe(this ErrorResponse response)
    {
        return (
            Transcriptions.TryGetValue(response.Status, out var transcriber)
                ? transcriber(response)
                : "Untranscribable error occured"
        ).ToUpper();
    }
}