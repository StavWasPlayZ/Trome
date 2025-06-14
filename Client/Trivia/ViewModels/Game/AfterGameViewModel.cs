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
    private const int MockUsers = 30;
    
    public List<PlayerResultModel> Results { get; }

    public PlayerResultModel WinnerResults { get; private set; } = null!;
    public List<PlayerResultModel>? List2Results { get; private set; }
    public List<PlayerResultModel>? List3Results { get; private set; }
    

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
        Results = Enumerable.Range(1, MockUsers)
            .Select(User.CreateMock)
            .Select(user => new PlayerResultModel
            {
                Place = user.Id,
                
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
            : null;

        List3Results = Results.Count > 3
            ? Results[3..]
            : null;
    }
    
    
    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case GameStartedNotification:
                NavigateAndPop(new GameCountdownViewModel(HostScreen, RoomModel))!.Subscribe();
                break;
        }
        
        base.CommOnPacketReceived(packet);
    }
}
