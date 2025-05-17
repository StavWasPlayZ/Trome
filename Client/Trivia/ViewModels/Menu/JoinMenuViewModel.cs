using System.Reactive;
using ReactiveUI;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Menu;

public class JoinMenuViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; } = NavigateReactiveCommand(
        () => new CreateRoomViewModel(hostScreen)
    );
    
    public ReactiveCommand<Unit, IRoutableViewModel> JoinRoomButtonCommand { get; } = NavigateReactiveCommand(
        () => new JoinedRoomViewModel(hostScreen)
    );
}
