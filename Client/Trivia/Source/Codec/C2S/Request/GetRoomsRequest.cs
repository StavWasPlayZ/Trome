namespace Trivia.Codec.C2S.Request;

public record GetRoomsRequest() 
    : ProtocolRequest(RequestCode.GetRooms);