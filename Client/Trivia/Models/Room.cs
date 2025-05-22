using Trivia.Codec;

namespace Trivia.Models;

public class Room
{
    public required int Id { get; init; }
    public required RoomStatus Status { get; init; }
    public required User Admin { get; init; }
    
    public required int PlayersCount { get; init; }
    
    public required RoomData Data { get; init; }
}
