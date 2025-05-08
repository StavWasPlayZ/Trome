using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class JoinMenuViewModel : PageViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel>? BackButtonCommand { get; } =
        GetMainWindowViewModel()?.Router.NavigateBack;
    
    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; }
    
    public JoinMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        NewRoomButtonCommand = ReactiveCommand.CreateFromObservable(() => NavigateTo(
            //TODO: Have this:
            // new CreateRoomViewModel(HostScreen))!
            
            new MainMenuViewModel(HostScreen))!
        );
    }
}
