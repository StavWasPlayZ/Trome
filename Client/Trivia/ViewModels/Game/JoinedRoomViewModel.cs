using System.Collections.Generic;
using ReactiveUI;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class JoinedRoomViewModel : RoomViewModel
{
    public JoinedRoomViewModel(IScreen hostScreen, Room room, List<User> players) : base(hostScreen, room, players)
    {
    }

    public JoinedRoomViewModel()
    {
    }
}