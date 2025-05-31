using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;

namespace Trivia.ViewModels.Game;

public class GameViewModelBase : PageViewModel
{
    public ReactiveCommand<Unit, Unit> LeaveGameCommand { get; }

    public GameViewModelBase(IScreen hostScreen) : base(hostScreen)
    {
        LeaveGameCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<LeaveGameResponse>(new LeaveGameRequest());
            
            // Assuming Join -> Room -> Game
            NavigateBackCommand!.Execute();
            NavigateBackCommand!.Execute();
        });
    }

    public GameViewModelBase()
    {
        LeaveGameCommand = NoOpCommand;
    }
}