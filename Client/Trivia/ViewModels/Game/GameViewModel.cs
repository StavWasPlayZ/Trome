using ReactiveUI;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class GameViewModel : PageViewModel
{
    public RoomData Data { get; }
    
    public GameViewModel(IScreen hostScreen, RoomData data) : base(hostScreen)
    {
        Data = data;
    }

    public GameViewModel()
    {
        Data = Room.CreateMockRoom(AppService.SessionUser!).Data;
    }
}
