using System;
using System.Reactive;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models;
using Trivia.Models.Raw;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Room;

public class RoomAdminViewModel : RoomViewModel
{
    public ReactiveCommand<Unit, Unit> StartGameCommand { get; }

    public RoomAdminViewModel(IScreen hostScreen, RoomModel roomModel) : base(hostScreen, roomModel, [])
    {
        _name = roomModel.Data.Name;
        _questions = roomModel.Data.QuestionsCount;
        _secsPerQuestion = roomModel.Data.TimePerQuestionSecs;
        MaxPlayers = roomModel.Data.MaxPlayers;

        StartGameCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAsync<StartGameResponse>(new StartGameRequest(RoomModel.Data));

            NavigateTo(new GameCountdownViewModel(HostScreen, RoomModel));
        });

        
        this.WhenActivated(disposables =>
        {
            this
                .WhenAnyValue(
                    x => x.Name,
                    x => x.Questions,
                    x => x.SecsPerQuestion,
                    x => x.MaxPlayers
                )
                .Skip(1) // Skip initialization invocation
                .DistinctUntilChanged()
                .Throttle(RoomDataUpdateDelay)
                .Where(_ => IsValidRoomData)
                .Subscribe(_ => SendRoomData())
                .DisposeWith(disposables);
            
            
            // Validations
            
            this
                .WhenAnyValue(x => x.SecsPerQuestion)
                .Subscribe(_ => SecsPerQuestionValid = SecsPerQuestion > 0)
                .DisposeWith(disposables);
            
            this
                .WhenAnyValue(x => x.MaxPlayers)
                .Subscribe(_ => MaxPlayersValid = MaxPlayers > 0)
                .DisposeWith(disposables);
        });
    }

    public RoomAdminViewModel()
    {
        _name = "ROOM NAME";
        _questions = 20;
        _secsPerQuestion = 10;
        MaxPlayers = 10;
        
        StartGameCommand = NoOpCommand;
    }


    private bool _secsPerQuestionValid = true;

    public bool SecsPerQuestionValid
    {
        get => _secsPerQuestionValid;
        set => this.RaiseAndSetIfChanged(ref _secsPerQuestionValid, value);
    }
    
    private bool _maxPlayersValid = true;

    public bool MaxPlayersValid
    {
        get => _maxPlayersValid;
        set => this.RaiseAndSetIfChanged(ref _maxPlayersValid, value);
    }


    private bool IsValidRoomData => SecsPerQuestionValid && MaxPlayersValid;

    private void SendRoomData()
    {
        UpdateAndSendRoomData(new RoomData
        {
            Name = _name,
            QuestionsCount = _questions,
            MaxPlayers = MaxPlayers,
            TimePerQuestionSecs = _secsPerQuestion
        });
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
}
