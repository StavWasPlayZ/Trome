using System.Reactive;
using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.ViewModels;

public abstract class AuthViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
    public ReactiveCommand<Unit, IRoutableViewModel> AuthenticateCommand { get; } = NavigateAndResetReactiveCommand(
        () => new MainMenuViewModel(hostScreen)
    );
}