using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class CreateRoomViewModel : RoomViewModel
{
    public ReactiveCommand<Unit, Unit> CloseRoomCommand { get; }

    public CreateRoomViewModel(IScreen hostScreen, Room room) : base(hostScreen, room)
    {
        _name = room.Data.Name;
        _questions = room.Data.QuestionsCount;
        _secsPerQuestion = room.Data.TimePerQuestionSecs;
        _maxPlayers = room.Data.MaxPlayers;
        
        CloseRoomCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<CloseRoomResponse>(new CloseRoomRequest());
            
            NavigateBackCommand!.Execute();
        });
    }

    public CreateRoomViewModel()
    {
        _name = "ROOM NAME";
        _questions = 20;
        _secsPerQuestion = 10;
        _maxPlayers = 10;
        
        CloseRoomCommand = NoOpCommand;
    }



    private string _name;

    public string Name
    {
        get => _name;
        set => this.RaiseAndSetIfChanged(ref _name, value);
    }
    
    
    private int _questions;

    public int Questions
    {
        get => _questions;
        set => this.RaiseAndSetIfChanged(ref _questions, value);
    }
    
    
    private int _secsPerQuestion;

    public int SecsPerQuestion
    {
        get => _secsPerQuestion;
        set => this.RaiseAndSetIfChanged(ref _secsPerQuestion, value);
    }


    private int _maxPlayers;

    public int MaxPlayers
    {
        get => _maxPlayers;
        set => this.RaiseAndSetIfChanged(ref _maxPlayers, value);
    }
}
