using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class SignupViewModel(IScreen hostScreen) : AuthViewModel(hostScreen)
{
    public ReactiveCommand<Unit, IRoutableViewModel> ToLoginCommand { get; } = NavigateAndResetReactiveCommand(
        () => new LoginViewModel(hostScreen)
    );
}