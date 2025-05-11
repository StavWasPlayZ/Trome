namespace Trivia.Codec.C2S.Request;

public record CreateRoomRequest(
    string RoomName,
    string MaxPlayers,
    string QuestionCount,
    string AnswerTimeout
) : ProtocolRequest(RequestCode.CreateRoom);