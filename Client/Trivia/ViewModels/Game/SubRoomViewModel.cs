using System;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public abstract class SubRoomViewModel : PageViewModel
{
    public RoomModel RoomModel { get; }

    protected SubRoomViewModel(IScreen hostScreen, RoomModel roomModel) : base(hostScreen)
    {
        RoomModel = roomModel;
    }

    protected SubRoomViewModel()
    {
        RoomModel = RoomModel.CreateMockRoom(AppService.SessionUser!);
    }
    
    
    protected void NavBackFromRoom()
    {
        // Assuming Join -> Room -> Game
        NavigateBackCommand!.Execute().Subscribe();
        NavigateBackCommand!.Execute().Subscribe();
    }
    
    
    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case RoomClosedNotification:
                NavBackFromRoom();
                break;
            
            case PlayerKickedNotification playerKickedNotif:
                HandlePlayerKicked(playerKickedNotif);
                break;
        }
        
        base.CommOnPacketReceived(packet);
    }

    private void HandlePlayerKicked(PlayerKickedNotification playerKickedNotif)
    {
        if (playerKickedNotif.UserId == AppService.SessionUser!.Id)
        {
            NavBackFromRoom();
        }
    }
}