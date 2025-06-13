namespace Trivia.Codec.S2C.Notification.Packets;

public readonly record struct PlayerKickedNotification(
    int UserId
) : IProtocolNotification;
