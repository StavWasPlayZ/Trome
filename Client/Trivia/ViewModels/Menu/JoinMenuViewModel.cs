using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class JoinMenuViewModel : PageViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; }
    
    public JoinMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        NewRoomButtonCommand = NavigationReactiveCommand(new CreateRoomViewModel(HostScreen));
    }
}
