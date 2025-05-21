using Trivia.Codec;

namespace Trivia.Models;

public record Room(
    int Id,
    RoomStatus Status,
    User Admin,
    
    int PlayersCount,
    
    RoomData Data
);
