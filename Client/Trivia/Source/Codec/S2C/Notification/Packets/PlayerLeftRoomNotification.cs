namespace Trivia.Codec.S2C.Notification.Packets;

public readonly record struct PlayerLeftRoomNotification(
    long PlayerId
) : IProtocolNotification;
