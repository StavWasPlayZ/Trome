using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class MainMenuViewModel : PageViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> PlayCommand { get; }
    
    public MainMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        PlayCommand = ReactiveCommand.CreateFromObservable(() => NavigateTo(new JoinMenuViewModel(HostScreen))!);
    }
}