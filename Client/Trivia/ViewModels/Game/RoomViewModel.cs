using System.Collections.Generic;
using System.Linq;
using ReactiveUI;
using Trivia.Models.Raw;
using Trivia.Models.User;

namespace Trivia.ViewModels.Game;

public abstract class RoomViewModel : PageViewModel
{
    public Room Room { get; init; }
    public List<RoomUserModel?> Users { get; }

    protected RoomViewModel(IScreen hostScreen, Room room) : base(hostScreen)
    {
        Room = room;
        
        Users = new List<RoomUserModel?>(room.Data.MaxPlayers)
        {
            RoomUserModel.FromUser(room.Admin, room.Admin == AppService.SessionUser, true)
        };

        for (var i = 1; i < room.Data.MaxPlayers; i++)
        {
            Users.Add(null);
        }
    }

    protected RoomViewModel()
    {
        Room = Room.CreateMockRoom(AppService.SessionUser!);
        
        Users = Enumerable.Range(1, 10)
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