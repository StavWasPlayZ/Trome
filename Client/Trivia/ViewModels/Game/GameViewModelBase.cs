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
    

    protected GameViewModelBase(IScreen hostScreen, RoomModel roomModel) : base(hostScreen, roomModel)
    {        
        LeaveGameCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAsync<LeaveGameResponse>(new LeaveGameRequest());
            NavBackFromRoom();
        });
    }

    protected GameViewModelBase()
    {
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
            // When a player leaves, it is also to be considered that they have finished.
            case PlayerLeftRoomNotification:
            case PlayerFinishedNotification:
                PlayersFinished++;
                break;
            
            case PlayerKickedNotification playerKickedNotif:
                HandlePlayerKicked(playerKickedNotif);
                break;
        }
        
        base.CommOnPacketReceived(packet);
    }
    
    private void HandlePlayerKicked(PlayerKickedNotification playerKickedNotif)
    {
        if (playerKickedNotif.PlayerId == AppService.SessionUser!.Id)
            return;
        
        PlayersFinished++;
    }
}