using System.Collections.Generic;
using System.Linq;
using ReactiveUI;
using Trivia.Models.Raw;
using Trivia.Models.User;

namespace Trivia.ViewModels.Game;

public class CreateRoomViewModel : PageViewModel
{
    public Room Room { get; init; }
    public List<RoomUserModel?> Users { get; }

    public CreateRoomViewModel(IScreen hostScreen, Room room) : base(hostScreen)
    {
        Room = room;
        
        Users = new List<RoomUserModel?>(room.Data.MaxPlayers)
        {
            RoomUserModel.FromUser(AppService.SessionUser!, true, true)
        };

        for (var i = 1; i < room.Data.MaxPlayers; i++)
        {
            Users.Add(null);
        }
    }

    public CreateRoomViewModel()
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