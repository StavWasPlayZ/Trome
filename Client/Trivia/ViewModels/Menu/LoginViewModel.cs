using System;
using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class LoginViewModel(IScreen hostScreen) : AuthViewModel(hostScreen)
{
    public ReactiveCommand<Unit, IRoutableViewModel> ToSignupCommand { get; } = NavigateAndResetReactiveCommand(
        () => new SignupViewModel(hostScreen)
    );

    protected override void UpdateMayAuthenticate()
    {
        throw new NotImplementedException();
    }
}