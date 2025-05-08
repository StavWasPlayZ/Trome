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

        /// <summary>
        /// S - The enum Status type
        /// </summary>
        public class ProtocolRequest<S>
        {
            public ProtocolRequest(S status)
            {
                this.Status = status;
            }
            public readonly S Status;
        }
    }
}