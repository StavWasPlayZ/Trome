using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.ViewModels.Auth;

namespace Trivia.ViewModels.Menu;

public class MainMenuViewModel : PageViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> PlayCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> StatisticsCommand { get; }
    public ReactiveCommand<Unit, IRoutableViewModel> AboutCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> SettingsCommand { get; }
    
    public ReactiveCommand<Unit, Unit> LogOutCommand { get; }

    public ReactiveCommand<Unit, Unit> ExitCommand { get; }
    
    public MainMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        PlayCommand = NavigateReactiveCommand(
            () => new JoinRoomMenuViewModel(hostScreen)
        );
        
        StatisticsCommand = NavigateReactiveCommand(
            () => new StatisticsViewModel(hostScreen)
        );

        AboutCommand = NavigateReactiveCommand(
            () => new AboutViewModel(hostScreen)
        );
        
        LogOutCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAsync<LogoutResponse>(new LogoutRequest());
        
            // Assuming this just worked.
            NavigateAndReset(new LoginViewModel(hostScreen));
        });
        
        SettingsCommand = NavigateReactiveCommand(
            () => new SettingsMenuViewModel(hostScreen)
        );

        ExitCommand = ReactiveCommand.Create(() =>
        {
            MainWindow.Instance!.Close();
        });
    }

    public MainMenuViewModel()
    {
        SettingsCommand = PlayCommand = StatisticsCommand = NoOpNavCommand;
        ExitCommand = LogOutCommand = NoOpCommand;
    }
}