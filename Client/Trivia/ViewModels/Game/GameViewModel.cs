using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive;
using System.Reactive.Disposables;
using System.Threading.Tasks;
using System.Web;
using Avalonia.Threading;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Objects;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Codec.S2C.Response.Packets.Impl;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public class GameViewModel : GameViewModelBase
{
    private static readonly TimeSpan CountdownSleep = TimeSpan.FromSeconds(1/60.0);
    
    public ReactiveCommand<int, Unit> SubmitAnswerCommand { get; }
    
    private DispatcherTimer? _countdownTimer;
    
    
    public GameViewModel(IScreen hostScreen, RoomModel roomModel) : base(hostScreen, roomModel)
    {
        SubmitAnswerCommand = ReactiveCommand.CreateFromTask<int>(async (btnIndex, _) =>
            await SubmitAnswer(btnIndex)
        );
        
        _countdownTimer = new DispatcherTimer
        {
            Interval = CountdownSleep
        };
        
        this.WhenActivated(disposables =>
        {
            App.MusicService?.PlayTriviaTrack();
            
            GetNewQuestion()
                .DisposeWith(disposables);
            
            _countdownTimer.Tick += CountdownTicked;
            Disposable
                .Create(() =>
                {
                    _countdownTimer.Tick -= CountdownTicked;
                    _countdownTimer.Stop();
                })
                .DisposeWith(disposables);
        });
    }

    public GameViewModel()
    {
        _timeLeft = TimeSpan.FromSeconds(RoomModel.Data.TimePerQuestionSecs - 1);
        Points = 4269;
        _leadingUsername = "Username";
        
        _question = QuestionModel.MockQuestionModel;
        UpdateHalvedButtons();

        SubmitAnswerCommand = ReactiveCommand.Create<int>(_ => { });
    }


    private async Task GetNewQuestion()
    {
         var response = await Comm.SendRequestAwaitResponse<GetQuestionResponse>(new GetQuestionRequest());
         Question = response.Question;
         Points = response.Points;
         
         HandleQuestion(response);
    }

    
    private async Task SubmitAnswer(int btnIndex)
    {
        StopCountdown();
        
        var response = await Comm.SendRequestAwaitResponse<SubmitAnswerResponse>(new SubmitAnswerRequest(btnIndex));
        Question = response.Question;
        Points = response.Points;

        CurrQuestionCount++;
        HandleQuestion(response);
    }


    
    private void StartCountdown()
    {
        TimeLeft = TimeSpan.FromSeconds(RoomModel.Data.TimePerQuestionSecs);
        _countdownTimer!.Start();
    }
    
    private void StopCountdown()
    {
        _countdownTimer!.Stop();
    }

    private void CountdownTicked(object? sender, EventArgs eventArgs)
    {
        TimeLeft -= CountdownSleep;
        
        if (TimeLeft <= TimeSpan.Zero)
        {
            _countdownTimer!.Stop();
            _ = HandleCountdownEnded();
        }
    }

    private async Task HandleCountdownEnded()
    {        
        CurrQuestionCount++;
        await GetNewQuestion();
    }

    
    private void HandleQuestion(IQuestionResponse response)
    {
        if (Question == null)
        {
            HandleLastQuestion(response.Results!);
            return;
        }

        Question = new QuestionModel
        {
            Prompt = HttpUtility.HtmlDecode(Question.Prompt),
            Answers = Question.Answers
                // This is not a nullable.
                // ReSharper disable once ConvertClosureToMethodGroup
                .Select(answer => HttpUtility.HtmlDecode(answer))
                .ToList()
        };

        UpdateHalvedButtons();
        StartCountdown();
    }

    private void HandleLastQuestion(IList<PlayerResult> results)
    {
        PlayersFinished++;
        
        // If the below is true, then we were the last player
        // to have finished the game.
        if (PlayersFinished == RoomModel.PlayersCount)
        {
            App.MusicService?.PlayBackgroundTrack();
            NavigateAndPop(new AfterGameViewModel(HostScreen, RoomModel, results))!.Subscribe();
        }
        else
        {
            NavigateAndPop(new FinishedEarlyViewModel(HostScreen, RoomModel, PlayersFinished))!.Subscribe(); 
        }
    }


    private void UpdateHalvedButtons()
    {
        HalvedBtnTexts = Question!.Answers
            .Select(answer => answer.Length > 20)
            .ToArray();
    }
    
    
    private int _currQuestionCount = 1;

    public int CurrQuestionCount
    {
        get => _currQuestionCount;
        set => this.RaiseAndSetIfChanged(ref _currQuestionCount, value);
    }


    private QuestionModel? _question;

    public QuestionModel? Question
    {
        get => _question;
        private set => this.RaiseAndSetIfChanged(ref _question, value);
    }
    
    
    private int _points;

    public int Points
    {
        get => _points;
        set => this.RaiseAndSetIfChanged(ref _points, value);
    }
    
    
    private TimeSpan _timeLeft;

    public TimeSpan TimeLeft
    {
        get => _timeLeft;
        set => this.RaiseAndSetIfChanged(ref _timeLeft, value);
    }


    private string? _leadingUsername;

    public string? LeadingUsername
    {
        get => _leadingUsername;
        set => this.RaiseAndSetIfChanged(ref _leadingUsername, value);
    }


    private bool[] _halvedBtnTexts = new bool[4];

    public bool[] HalvedBtnTexts
    {
        get => _halvedBtnTexts;
        private set => this.RaiseAndSetIfChanged(ref _halvedBtnTexts, value);
    }
}
