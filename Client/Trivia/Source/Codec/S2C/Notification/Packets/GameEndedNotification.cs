using Trivia.Codec.S2C.Objects;

namespace Trivia.Codec.S2C.Notification.Packets;

public readonly record struct GameEndedNotification(PlayerResult[] Results) : IProtocolNotification;
