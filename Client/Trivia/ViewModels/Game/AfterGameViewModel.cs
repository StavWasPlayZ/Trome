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
    public List<PlayerResultModel> Results { get; }

    public PlayerResultModel WinnerResults { get; private set; } = null!;
    public List<PlayerResultModel> List2Results { get; private set; } = null!;
    public List<PlayerResultModel> List3Results { get; private set; } = null!;
    

    public AfterGameViewModel(IScreen hostScreen, RoomModel roomModel, IList<PlayerResult> results) :
        base(hostScreen, roomModel)
    {
        // Sort by points
        List<PlayerResult> sortedResults = [..results];
        sortedResults.Sort((prev, curr) => curr.Points.CompareTo(prev.Points));
        
        Results = sortedResults
            .Select((result, i) => PlayerResultModel.FromPlayerResult(result, i + 1))
            .ToList();
        
        InitShorthandLists();
    }

    public AfterGameViewModel()
    {
        Results = Enumerable.Range(0, 30)
            .Select(i => new User
            {
                Id = i,
                Username = $"User {i + 1}",
            })
            .Select(user => new PlayerResultModel
            {
                Place = user.Id + 1,
                
                User = user,
                PlaytimeSecs = 123,
                AverageAnswerTimeSecs = 15,
                CorrectAnswerCount = 5,
                Points = 69420
            })
            .ToList();
        
        InitShorthandLists();
    }

    private void InitShorthandLists()
    {
        WinnerResults = Results[0];
        
        List2Results = Results.Count > 1
            ? Results[1..Math.Min(3, Results.Count)]
            : [];

        List3Results = Results.Count > 3
            ? Results[3..]
            : [];
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
