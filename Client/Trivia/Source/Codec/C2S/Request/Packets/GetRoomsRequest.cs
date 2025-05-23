namespace Trivia.Codec.C2S.Request.Packets;

public record GetRoomsRequest() 
    : ProtocolRequest(RequestCode.GetRooms);