using System;
using System.Collections.Generic;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Codec.S2C.Objects;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public class AfterGameViewModel : SubRoomViewModel
{
    public IList<PlayerResult> Results { get; }

    public AfterGameViewModel(IScreen hostScreen, RoomModel roomModel, IList<PlayerResult> results) :
        base(hostScreen, roomModel)
    {
        Results = results;
    }

    public AfterGameViewModel()
    {
        Results = [
            // new PlayerResult()
        ];
    }
    
    
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