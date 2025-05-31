using Trivia.Codec.S2C.Notification.Packets;

namespace Trivia.Codec.S2C.Notification;

public static class NotificationDeserializer
{
    public static IProtocolNotification? Deserialize(NotificationCode code, string json)
    {
        return code switch
        {
            NotificationCode.PlayerJoinedRoom => PacketDeserializer.Deserialize<PlayerJoinedRoomNotification>(json),
            NotificationCode.PlayerLeftRoom => PacketDeserializer.Deserialize<PlayerLeftRoomNotification>(json),
            NotificationCode.RoomClosed => PacketDeserializer.Deserialize<RoomClosedNotification>(json),
            NotificationCode.RoomDataUpdated => PacketDeserializer.Deserialize<RoomDataUpdatedNotification>(json),
            NotificationCode.GameStarted => PacketDeserializer.Deserialize<GameStartedNotification>(json),
            NotificationCode.GameEnded => PacketDeserializer.Deserialize<GameEndedNotification>(json),
            NotificationCode.PlayerFinished => PacketDeserializer.Deserialize<PlayerFinishedNotification>(json),
            _ => null
        };
    }
}
