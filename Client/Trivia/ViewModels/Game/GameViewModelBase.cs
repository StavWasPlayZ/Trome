using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public abstract class GameViewModelBase : SubRoomViewModel
{
    public ReactiveCommand<Unit, Unit> LeaveGameCommand { get; }
    
    public RoomModel RoomModel { get; }

    protected GameViewModelBase(IScreen hostScreen, RoomModel roomModel) : base(hostScreen)
    {
        RoomModel = roomModel;
        
        LeaveGameCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<LeaveGameResponse>(new LeaveGameRequest());
            await NavBackFromRoom();
        });
    }

    protected GameViewModelBase()
    {
        RoomModel = RoomModel.CreateMockRoom(AppService.SessionUser!);

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