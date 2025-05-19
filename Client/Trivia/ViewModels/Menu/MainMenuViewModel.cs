using System.Reactive;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C.Response;
using Trivia.ViewModels.Auth;

namespace Trivia.ViewModels.Menu;

public class MainMenuViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
    public ReactiveCommand<Unit, IRoutableViewModel> PlayCommand { get; } = NavigateReactiveCommand(
        () => new JoinMenuViewModel(hostScreen)
    );
    
    public ReactiveCommand<Unit, IRoutableViewModel> StatisticsCommand { get; } = NavigateReactiveCommand(
        () => new StatisticsViewModel(hostScreen)
    );
    
    // public ReactiveCommand<Unit, IRoutableViewModel> LogOutCommand { get; } = NavigateAndResetReactiveCommand(
    //     () => new LoginViewModel(hostScreen)
    // );
    public ReactiveCommand<Unit, Unit> LogOutCommand { get; } = ReactiveCommand.CreateFromTask(async () =>
    {
        await Comm.SendRequestAwaitResponse<LogoutResponse>(new LogoutRequest());
        
        // Assuming this just worked.
        NavigateAndReset(new LoginViewModel(hostScreen));
    });

    public ReactiveCommand<Unit, Unit> ExitCommand { get; } = ReactiveCommand.Create(() =>
    {
        (Application.Current?.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime)
            ?.Shutdown();
    });
}