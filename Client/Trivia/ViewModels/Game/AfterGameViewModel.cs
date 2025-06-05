using System;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public class AfterGameViewModel : SubRoomViewModel
{
    public AfterGameViewModel(IScreen hostScreen, RoomModel roomModel) : base(hostScreen, roomModel)
    {}

    public AfterGameViewModel()
    {}
    
    
    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case RoomClosedNotification:
                NavBackFromRoom().Wait();
                break;
            
            case GameStartedNotification:
                NavigateAndPop(new GameViewModel(HostScreen, RoomModel))!.Subscribe();
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
}