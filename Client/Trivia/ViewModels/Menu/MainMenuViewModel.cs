using System.Reactive;
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
        // TODO: Make this to go to settings
        // this is temp for looking if AddQuestion works
        SettingsCommand= NavigateReactiveCommand(
            () => new AddQuestionViewModel(hostScreen)
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
        PlayCommand = StatisticsCommand = SettingsCommand = NoOpNavCommand;
        LogOutCommand = NoOpCommand;
    }
    

    public ReactiveCommand<Unit, IRoutableViewModel> PlayCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> StatisticsCommand { get; }
    
    public ReactiveCommand<Unit, Unit> LogOutCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> SettingsCommand { get; }

    public ReactiveCommand<Unit, Unit> ExitCommand { get; } = ReactiveCommand.Create(() =>
    {
        MainWindow.ApplicationLifetime?.Shutdown();
    });
}