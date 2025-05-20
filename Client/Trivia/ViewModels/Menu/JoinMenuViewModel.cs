using System.Reactive;
using ReactiveUI;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Menu;

public class JoinMenuViewModel : PageViewModel
{
    public JoinMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        NewRoomButtonCommand = NavigateReactiveCommand(
            () => new CreateRoomViewModel(hostScreen)
        );
        JoinRoomButtonCommand = NavigateReactiveCommand(
            () => new JoinedRoomViewModel(hostScreen)
        );
    }

    public JoinMenuViewModel()
    {
        NewRoomButtonCommand = JoinRoomButtonCommand = NoOpNavCommand;
    }

    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> JoinRoomButtonCommand { get; }
}
