using ReactiveUI;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class CreateRoomViewModel : RoomViewModel
{
    public CreateRoomViewModel(IScreen hostScreen, Room room) : base(hostScreen, room)
    {
    }

    public CreateRoomViewModel()
    {
    }
}