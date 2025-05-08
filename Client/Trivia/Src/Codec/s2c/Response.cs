using System;
using System.Text;
using System.Text.Json;

namespace Trivia.Codec.s2c
{
    namespace Response
    {
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
    }
}