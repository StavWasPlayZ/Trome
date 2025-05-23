namespace Trivia.Codec.S2C.Notification.Packets;

public record PlayerLeftRoomNotification(
    int PlayerId
) : ProtocolNotification;
