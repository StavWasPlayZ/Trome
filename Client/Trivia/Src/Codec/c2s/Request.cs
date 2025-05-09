
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
            protected ProtocolRequest(RequestCode code)
            {
                this.Code = code;
            }
            public readonly RequestCode Code;
        }

        public class LoginRequest : ProtocolRequest
        {
            public LoginRequest(string username, string password) : base(RequestCode.Login)
            {
                this.Username = username;
                this.Password = password;
            }
            
            public readonly string Username;
            public readonly string Password;
        }

        public class SignupRequest : ProtocolRequest
        {
            public SignupRequest(string username, string password, string email, string phone, string? address, string birthday) : base(RequestCode.Signup)
            {
                this.Username = username;
                this.Password = password;
                this.Email = email;
                this.Phone = phone;
                this.Address = address;
                this.Birthday = birthday;
            }
            
            public readonly string Username;
            public readonly string Password;
            public readonly string Email;
            public readonly string Phone;
            public readonly string? Address;
            public readonly string Birthday;
        }

        public class GetPlayersInRoomRequest : ProtocolRequest
        {
            GetPlayersInRoomRequest(uint roomId) : base(RequestCode.GetPlayersInRoom)
            {
                this.RoomID = roomId;
            }
            public readonly uint RoomID;
        }
        
        public class JoinRoomRequest : ProtocolRequest
        {
            JoinRoomRequest(uint roomId) : base(RequestCode.JoinRoom)
            {
                this.RoomID = roomId;
            }
            public readonly uint RoomID;
        }
        
        public class GetRoomsRequest : ProtocolRequest
        {
            GetRoomsRequest() : base(RequestCode.GetRooms)
            { }
        }

        public class CreateRoomRequest : ProtocolRequest
        {
            CreateRoomRequest(string roomName, string maxPlayers, string questionCount, string answerTimeout) 
                : base(RequestCode.CreateRoom)
            {
                this.RoomName = roomName;
                this.MaxPlayers = maxPlayers;
                this.QuestionCount = questionCount;
                this.AnswerTimeout = answerTimeout;
            }
            
            public readonly string RoomName;
            public readonly string MaxPlayers;
            public readonly string QuestionCount;
            public readonly string AnswerTimeout;
        }
        
        public class GetHighScoresRequest : ProtocolRequest
        {
            GetHighScoresRequest() : base(RequestCode.GetHighScores)
            { }
        }
        
        public class GetPersonalStatisticsRequest : ProtocolRequest
        {
            GetPersonalStatisticsRequest() : base(RequestCode.GetPersonalStatistics)
            { }
        }
    }
}