using System;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public class AfterGameViewModel : SubRoomViewModel
{
    private RoomModel _room;

    public AfterGameViewModel(IScreen hostScreen, RoomModel room) : base(hostScreen)
    {
        _room = room;
    }

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
                NavigateAndPop(new GameViewModel(HostScreen, _room))!.Subscribe();
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
}