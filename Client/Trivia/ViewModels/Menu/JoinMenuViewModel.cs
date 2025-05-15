using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class JoinMenuViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; } = NavigateReactiveCommand(
        () => new CreateRoomViewModel(hostScreen)
    );
}
