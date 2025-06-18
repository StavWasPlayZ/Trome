using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public abstract class GameViewModelBase : SubRoomViewModel
{
    public ReactiveCommand<Unit, Unit> LeaveGameCommand { get; }
    

    protected GameViewModelBase(IScreen hostScreen, RoomModel roomModel, int playersFinished) :
        base(hostScreen, roomModel, playersFinished)
    {        
        LeaveGameCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            App.MusicService?.PlayBackgroundTrack();
            
            await Comm.SendRequestAsync<LeaveGameResponse>(new LeaveGameRequest());
            NavBackFromRoom();
        });
    }

    protected GameViewModelBase()
    {
        LeaveGameCommand = NoOpCommand;
    }
}