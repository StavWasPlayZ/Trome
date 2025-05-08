using System;
using System.Text;
using System.Text.Json;

namespace Trivia.Codec.c2s
{
    namespace Request
    {
        public enum RequestCode : byte
        {
            Logout = 0,
            Login = 1,
            Signup,
            GetPlayersInRoom,
            JoinRoom,
            CreateRoom,
            GetRooms,
            GetHighScores,
            GetPersonalStatistics
        }

        public class ProtocolRequest
        {
            public ProtocolRequest(RequestCode code)
            {
                this.Code = code;
            }
            public readonly RequestCode Code;
        }
    }
}