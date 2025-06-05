using System;
using System.Collections.Generic;
using System.Linq;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Codec.S2C.Objects;
using Trivia.Models;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class AfterGameViewModel : SubRoomViewModel
{
    public List<PlayerResult> Results { get; }

    public PlayerResult WinnerResults => Results[0];
    

    public AfterGameViewModel(IScreen hostScreen, RoomModel roomModel, IList<PlayerResult> results) :
        base(hostScreen, roomModel)
    {
        Results = [..results];
        
        // Sort by points
        Results.Sort((prev, curr) => curr.Points.CompareTo(prev.Points));
    }

    public AfterGameViewModel()
    {
        Results = Enumerable.Range(0, 30)
            .Select(i => new User
            {
                Id = i,
                Username = $"User {i}",
            })
            .Select(user => new PlayerResult
            {
                User = user,
                AverageAnswerTimeSecs = 123,
                CorrectAnswerCount = 5,
                Points = 69420
            })
            .ToList();
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