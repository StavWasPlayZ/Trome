using System;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public abstract class SubRoomViewModel : PageViewModel
{
    public RoomModel RoomModel { get; }

    protected SubRoomViewModel(IScreen hostScreen, RoomModel roomModel, int playersFinished) : base(hostScreen)
    {
        RoomModel = roomModel;
        PlayersFinished = playersFinished;
    }

    protected SubRoomViewModel()
    {
        RoomModel = RoomModel.CreateMockRoom(AppService.SessionUser!);
        _playersFinished = 2;
    }
    
    
    protected void NavBackFromRoom()
    {
        // Assuming Join -> Room -> Game
        NavigateBackCommand!.Execute().Subscribe();
        NavigateBackCommand!.Execute().Subscribe();
    }
    
    
    private int _playersFinished;

    public int PlayersFinished
    {
        get => _playersFinished;
        set => this.RaiseAndSetIfChanged(ref _playersFinished, value);
    }
    
    
    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        base.CommOnPacketReceived(packet);

        switch (packet)
        {
            case RoomClosedNotification:
                NavBackFromRoom();
                break;
            
            // When a player leaves, it is also to be considered that they have finished.
            case PlayerLeftRoomNotification:
            case PlayerFinishedNotification:
                PlayersFinished++;
                break;
            
            case PlayerKickedNotification playerKickedNotif:
                HandlePlayerKicked(playerKickedNotif);
                break;
        }        
    }

    private void HandlePlayerKicked(PlayerKickedNotification playerKickedNotif)
    {
        if (playerKickedNotif.PlayerId == AppService.SessionUser!.Id)
        {
            NavBackFromRoom();
            return;
        }
        
        PlayersFinished++;
    }
}