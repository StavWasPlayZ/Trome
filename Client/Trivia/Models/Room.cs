using Trivia.Codec;

namespace Trivia.Models;

public record Room(
    int Id,
    string Name,
    RoomStatus Status,
    
    User Admin,
    
    int MaxPlayers,
    int TimePerQuestion,
    int PlayersCount,
    int Questions
);