using Trivia.Codec.S2C.Notification.Packets;

namespace Trivia.Codec.S2C.Notification;

public static class NotificationDeserializer
{
    public static ProtocolNotification? Deserialize(NotificationCode code, string json)
    {
        return code switch
        {
            NotificationCode.PlayerJoinedRoom => PacketDeserializer.Deserialize<PlayerJoinedRoomNotification>(json),
            NotificationCode.PlayerLeftRoom => PacketDeserializer.Deserialize<PlayerLeftRoomNotification>(json),
            NotificationCode.RoomClosed => PacketDeserializer.Deserialize<RoomClosedNotification>(json),
            NotificationCode.RoomDataUpdated => PacketDeserializer.Deserialize<RoomDataUpdatedNotification>(json),
            _ => null
        };
    }
}
