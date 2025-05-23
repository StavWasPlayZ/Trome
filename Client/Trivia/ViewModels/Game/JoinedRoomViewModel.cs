using ReactiveUI;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class JoinedRoomViewModel : RoomViewModel
{
    public JoinedRoomViewModel(IScreen hostScreen, Room room) : base(hostScreen, room)
    {
    }

    public JoinedRoomViewModel()
    {
    }
}