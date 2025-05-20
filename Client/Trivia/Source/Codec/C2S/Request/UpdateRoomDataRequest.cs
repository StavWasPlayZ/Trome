namespace Trivia.Codec.C2S.Request;

public record UpdateRoomDataRequest(
        string RoomName,
        int MaxPlayers,
        int TimePerQuestion,
        int QuestionCount
    ) : ProtocolRequest(RequestCode.UpdateRoomData);