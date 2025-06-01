using System.Reactive;
using System.Reactive.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public abstract class GameViewModelBase : PageViewModel
{
    public ReactiveCommand<Unit, Unit> LeaveGameCommand { get; }
    
    public Room Room { get; }

    protected GameViewModelBase(IScreen hostScreen, Room room) : base(hostScreen)
    {
        Room = room;
        
        LeaveGameCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<LeaveGameResponse>(new LeaveGameRequest());
            
            // Assuming Join -> Room -> Game
            await NavigateBackCommand!.Execute().ToTask();
            await NavigateBackCommand!.Execute().ToTask();
        });
    }

    protected GameViewModelBase()
    {
        Room = Room.CreateMockRoom(AppService.SessionUser!);

        LeaveGameCommand = NoOpCommand;
        _playersFinished = 2;
    }
    
    
    private int _playersFinished;

    public int PlayersFinished
    {
        get => _playersFinished;
        set => this.RaiseAndSetIfChanged(ref _playersFinished, value);
    }
    
    
    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case PlayerFinishedNotification:
                PlayersFinished++;
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
}