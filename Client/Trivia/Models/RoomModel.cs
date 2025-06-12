using System.Collections.Generic;
using System.Linq;
using Trivia.Codec;
using Trivia.Codec.C2S;
using Trivia.Models.Raw;

namespace Trivia.Models;

public record RoomModel : IdentifiableModel
{
    public RoomStatus Status { get; init; } = RoomStatus.Waiting;
    public required Raw.User Admin { get; init; }
    public required RoomType RoomType { get; init; }
    
    public int PlayersCount { get; init; }
    
    public required RoomData Data { get; init; }


    public static List<RoomModel> GenerateMockRooms(int count)
    {
        return Enumerable.Range(1, count)
            .Select(i =>
            {
                var roomType = i % 3 == 0 ? RoomType.HeadToHead : RoomType.TriviaRush;
                
                return new RoomModel
                {
                    Id = i,
                    Status = RoomStatus.Waiting,
                    RoomType = roomType,

                    Admin = new Raw.User
                    {
                        Id = i,
                        Username = $"User {i}"
                    },

                    PlayersCount = roomType == RoomType.HeadToHead ? 1 : 2,

                    Data = RoomData.CreateMock($"Room {i}", roomType)
                };
            }).ToList();
    }

    public static RoomModel CreateMockRoom(Raw.User admin)
    {
        return new RoomModel
        {
            Id = 0,
            Admin = admin,
            RoomType = RoomType.TriviaRush,

            PlayersCount = 2,

            Data = RoomData.CreateMock("Room 0", RoomType.TriviaRush)
        };
    }
}
