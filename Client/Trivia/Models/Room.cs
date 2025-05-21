using Trivia.Codec;

namespace Trivia.Models;

public record Room(
    int Id,
    RoomStatus Status,
    User Admin,
    
    RoomMetadata Metadata
);