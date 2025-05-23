using ReactiveUI;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class CreateRoomViewModel : PageViewModel
{
    public Room Room { get; init; }

    public CreateRoomViewModel(IScreen hostScreen, Room room) : base(hostScreen)
    {
        Room = room;
    }

    public CreateRoomViewModel()
    {
        Room = Room.CreateMockRoom(AppService.SessionUser!);
    }
}