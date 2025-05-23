using System.Reactive;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.ViewModels.Auth;

namespace Trivia.ViewModels.Menu;

public class MainMenuViewModel : PageViewModel
{
    public MainMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        PlayCommand = NavigateReactiveCommand(
            () => new JoinRoomMenuViewModel(hostScreen)
        );
        StatisticsCommand = NavigateReactiveCommand(
            () => new StatisticsViewModel(hostScreen)
        );
        LogOutCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<LogoutResponse>(new LogoutRequest());
        
            // Assuming this just worked.
            NavigateAndReset(new LoginViewModel(hostScreen));
        });
    }

    public MainMenuViewModel()
    {
        PlayCommand = StatisticsCommand = NoOpNavCommand;
        LogOutCommand = NoOpCommand;
    }
    

    public ReactiveCommand<Unit, IRoutableViewModel> PlayCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> StatisticsCommand { get; }
    
    public ReactiveCommand<Unit, Unit> LogOutCommand { get; }

    public ReactiveCommand<Unit, Unit> ExitCommand { get; } = ReactiveCommand.Create(() =>
    {
        (Application.Current?.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime)
            ?.Shutdown();
    });
}