using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Notification.Packets;

public record PlayerJoinedRoomNotification(
    User Player
) : ProtocolNotification;
