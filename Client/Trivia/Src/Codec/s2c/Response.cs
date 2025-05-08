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

        /// <summary>
        /// S - The enum Status type
        /// </summary>
        public class ProtocolResponse<S>
        {
            protected ProtocolResponse(S status)
            {
                this.Status = status;
            }
            public readonly S Status;
        }

        /// <summary>
        /// S - The enum Status type
        /// </summary>
        public class RegistrationResponse<S> : ProtocolResponse<S>
        {
            protected RegistrationResponse(S status, uint userId) : base(status)
            {
                this.UserId = userId;
            }
            protected RegistrationResponse(S status) : base(status)
            {
                this.UserId = uint.MaxValue;
            }
            
            public readonly uint UserId;
        }
    }
}