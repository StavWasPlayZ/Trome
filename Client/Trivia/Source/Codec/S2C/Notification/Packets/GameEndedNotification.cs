using System.Collections.Immutable;
using Trivia.Codec.S2C.Objects;

namespace Trivia.Codec.S2C.Notification.Packets;

public readonly record struct GameEndedNotification(
    ImmutableList<PlayerResult> Results
) : IProtocolNotification;
