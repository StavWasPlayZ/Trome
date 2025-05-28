using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Notification.Packets;

public readonly record struct PlayerJoinedRoomNotification(
    User Player
) : IProtocolNotification;
