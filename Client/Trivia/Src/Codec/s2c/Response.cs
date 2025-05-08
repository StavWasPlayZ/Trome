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

        public enum LoginStatus : uint
        {
            Success = 1,
            FailedInvalidCredentials = 2,
            FailedAlreadyLoggedIn = 3,
            FailedInternalError = 0
        }

        public class LoginResponse : RegistrationResponse<LoginStatus>
        {
            public LoginResponse(LoginStatus status, uint userId) : base(status, userId) 
            {}
            
            /**
             * A failed login response. Provided no user ID.
             */
            public LoginResponse(LoginStatus status) : base(status)
            {}
        }

        public enum SignupStatus : uint
        {
            Success = 1,
            FailedUsernameTaken = 2,
            FailedInvalidArgument = 3,
            FailedInternalError = 0
        }

        public class SignupResponse : RegistrationResponse<SignupStatus>
        {
            public SignupResponse(SignupStatus status, uint userId) : base(status, userId)
            {}
            
            /**
             * A failed login response. Provided no user ID.
             */
            public SignupResponse(SignupStatus status) : base(status)
            {}
        }

        public enum LogoutStatus : uint
        {
            Success = 1,
            FailedNotLoggedIn = 2,
            FailedInternalError = 0
        }

        public class LogoutResponse : ProtocolResponse<LogoutStatus>
        {
            public LogoutResponse(LogoutStatus status) : base(status)
            {}
        }

        public enum ErrorStatus : uint
        {
            Generic = 0,
            ServerUnimplemented,
            IllegalRequest
        }

        public class ErrorResponse : ProtocolResponse<ErrorStatus>
        {
            public ErrorResponse(ErrorStatus status, string message) : base(status)
            {
                this.Message = message;
            }
            
            public readonly string Message;
        }
        
        // TODO - Get room metadata
        public class JoinRoomResponse : ProtocolResponse<ConsumingResponseStatus>
        {
            public JoinRoomResponse(ConsumingResponseStatus status) : base(status)
            {}
        }
    }
}