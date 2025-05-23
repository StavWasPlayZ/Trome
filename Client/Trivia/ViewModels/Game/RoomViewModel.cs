using System.Collections.Generic;
using System.Linq;
using ReactiveUI;
using Trivia.Models.Raw;
using Trivia.Models.User;

namespace Trivia.ViewModels.Game;

public abstract class RoomViewModel : PageViewModel
{
    public Room Room { get; init; }
    public List<RoomUserModel?> Players { get; }

    /// <summary>
    /// </summary>
    /// <param name="hostScreen"></param>
    /// <param name="room"></param>
    /// <param name="players">
    /// A list of already existing players.
    /// Must be present if the room's <see cref="Room.PlayersCount"/> is greater than 1.
    /// </param>
    protected RoomViewModel(IScreen hostScreen, Room room, List<User>? players = null) : base(hostScreen)
    {
        Room = room;
        
        // This assumes that the first player is always the admin.
        Players = new List<RoomUserModel?>(room.Data.MaxPlayers)
        {
            RoomUserModel.FromUser(room.Admin, room.Admin == AppService.SessionUser!, true)
        };

        for (var i = 1; i < room.PlayersCount; i++)
        {
            var player = players![i];
            Players.Add(RoomUserModel.FromUser(player, player == AppService.SessionUser!, false));
        }

        for (var i = room.PlayersCount; i < room.Data.MaxPlayers; i++)
        {
            Players.Add(null);
        }
    }

    protected RoomViewModel()
    {
        Room = Room.CreateMockRoom(AppService.SessionUser!);
        
        Players = Enumerable.Range(1, 10)
            .Select(i =>
            {
                if (i > 5)
                    return null;
                
                return new RoomUserModel
                {
                    Id = i,
                    IsAdmin = i == 1,
                    IsCurrentUser = i == 1,
                    Username = $"User {i}"
                };
            })
            .ToList();
    }
}