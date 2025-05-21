namespace Trivia.Codec.C2S.Request;

public record CreateRoomRequest() : ProtocolRequest(RequestCode.CreateRoom);