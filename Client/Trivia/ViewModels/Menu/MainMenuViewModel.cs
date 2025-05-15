using System.Reactive;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class MainMenuViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
    public ReactiveCommand<Unit, IRoutableViewModel> PlayCommand { get; } = NavigateReactiveCommand(
        () => new JoinMenuViewModel(hostScreen)
    );
    public ReactiveCommand<Unit, IRoutableViewModel> SignOutCommand { get; } = NavigateAndResetReactiveCommand(
        () => new LoginViewModel(hostScreen)
    );

    public ReactiveCommand<Unit, Unit> ExitCommand { get; } = ReactiveCommand.Create(() =>
    {
        (Application.Current?.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime)
            ?.Shutdown();
    });
}