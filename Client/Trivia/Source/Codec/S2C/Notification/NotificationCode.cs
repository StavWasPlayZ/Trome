namespace Trivia.Codec.S2C.Notification;

public enum NotificationCode : byte
{
    PlayerJoinedRoom,
    PlayerLeftRoom,
    RoomClosed,
    RoomDataUpdated,
    
    GameStarted,
    GameEnded,
    
    PlayerFinished,
    
    Kicked
}
