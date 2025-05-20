namespace Trivia.Models;

public enum RoomStatus : uint
{
    Waiting = 0,
    Playing = 1,
    NotFound = 2
}

public record Room(
    int Id,
    string Name,
    RoomStatus Status,
    
    User Admin,
    
    int MaxPlayers,
    int TimePerQuestion,
    int PlayersCount
);