using System.Reactive;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class MainMenuViewModel : PageViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> PlayCommand { get; }
    public ReactiveCommand<Unit, IRoutableViewModel> UsernameCommand { get; }
    
    public ReactiveCommand<Unit, Unit> ExitCommand { get; } = ReactiveCommand.Create(() =>
    {
        (Application.Current?.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime)
            ?.Shutdown();
    });
    
    public MainMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        PlayCommand = NavigationReactiveCommand(new JoinMenuViewModel(HostScreen));
        UsernameCommand = NavigationReactiveCommand(new SignupViewModel(HostScreen));
    }
}