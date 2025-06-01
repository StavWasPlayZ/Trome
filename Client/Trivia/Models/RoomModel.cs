using System.Collections.Generic;
using System.Linq;
using Trivia.Codec;

namespace Trivia.Models.Raw;

public record RoomModel : IdentifiableModel
{
    public RoomStatus Status { get; init; } = RoomStatus.Waiting;
    public required User Admin { get; init; }
    
    public int PlayersCount { get; init; }
    
    public required RoomData Data { get; init; }


    public static List<RoomModel> GenerateMockRooms(int count)
    {
        return Enumerable.Range(1, count)
            .Select(i => new RoomModel
                {
                    Id = i,
                    Status = i % 2 == 0 ? RoomStatus.Waiting : RoomStatus.Playing,
                    Admin = new User
                    {
                        Id = i,
                        Username = $"User {i}"
                    },

                    PlayersCount = 2,

                    Data = new RoomData
                    {
                        Name = $"Room {i}",
                        MaxPlayers = 10,
                        TimePerQuestionSecs = 7,
                        QuestionsCount = 15
                    }
                }
            ).ToList();
    }

    public static RoomModel CreateMockRoom(User admin)
    {
        return new RoomModel
        {
            Id = 0,
            Admin = admin,

            PlayersCount = 2,

            Data = new RoomData
            {
                Name = "Room 0",
                MaxPlayers = 10,
                TimePerQuestionSecs = 7,
                QuestionsCount = 15
            }
        };
    }
}
