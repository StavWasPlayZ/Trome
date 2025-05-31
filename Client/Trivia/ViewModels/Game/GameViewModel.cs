using System;
using System.Linq;
using System.Reactive;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Web;
using Avalonia.Threading;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class GameViewModel : GameViewModelBase
{
    private const int CountdownSleepMs = 10;
    
    public Room Room { get; }
    public ReactiveCommand<int, Unit> SubmitAnswerCommand { get; }
    
    private TaskCompletionSource? _countdownCompletion;
    
    public GameViewModel(IScreen hostScreen, Room data) : base(hostScreen)
    {
        Room = data;

        SubmitAnswerCommand = ReactiveCommand.CreateFromTask<int>(async (btnIndex, _) =>
            await SubmitAnswer(btnIndex)
        );
        
        this.WhenActivated(disposables =>
        {
            GetNewQuestion()
                .DisposeWith(disposables);
            
            this
                .WhenAnyValue(x => x.TimeLeft)
                .Skip(1)
                .Subscribe(_ => HandleTimeLeftChanged())
                .DisposeWith(disposables);
        });
    }

    public GameViewModel()
    {
        Room = Room.CreateMockRoom(AppService.SessionUser!);
        _timeLeft = TimeSpan.FromSeconds(Room.Data.TimePerQuestionSecs - 1);
        Points = 4269;
        _leadingUsername = "Username";
        
        _question = Question.MockQuestion;
        UpdateHalvedButtons();

        SubmitAnswerCommand = ReactiveCommand.Create<int>(_ => { });
    }


    private async Task GetNewQuestion()
    {
         var response = await Comm.SendRequestAwaitResponse<GetQuestionResponse>(new GetQuestionRequest());
         Question = response.Question;
         Points = response.Points;
         _finishedLast = response.WasLastPlayer;
         _playersFinished = response.PlayersFinished;
         
         HandleQuestion();
    }


    //NOTE: This is temporary until the players countdown (from Finished Early screen)
    // is implemented.
    private bool _finishedLast;
    
    private int _playersFinished;

    private async Task SubmitAnswer(int btnIndex)
    {
        await StopCountdown();
        
        var response = await Comm.SendRequestAwaitResponse<SubmitAnswerResponse>(new SubmitAnswerRequest(btnIndex));
        Question = response.NewQuestion;
        Points = response.Points;
        _finishedLast = response.WasLastPlayer;
        _playersFinished = response.PlayersFinished;

        CurrQuestionCount++;
        HandleQuestion();
    }


    private bool _countdownRunning;
    
    private void StartCountdown()
    {
        TimeLeft = TimeSpan.FromSeconds(Room.Data.TimePerQuestionSecs);
        
        _countdownRunning = true;
        _countdownCompletion = new TaskCompletionSource();
        new Thread(CountdownThread).Start();
    }
    
    private async Task StopCountdown()
    {
        _countdownRunning = false;
        await _countdownCompletion!.Task;
        _countdownCompletion = null;
    }

    private void CountdownThread()
    {
        _internalTimeLeft = TimeLeft;
        
        while (_countdownRunning)
        {
            Thread.Sleep(CountdownSleepMs);

            if (!_countdownRunning)
                break;
            
            _internalTimeLeft -= TimeSpan.FromMilliseconds(CountdownSleepMs);
            
            Dispatcher.UIThread.Post(() =>
            {
                TimeLeft = _internalTimeLeft;
            });
            
            if (_internalTimeLeft <= TimeSpan.Zero)
            {
                _countdownRunning = false;
            }
        }
        
        _countdownCompletion!.TrySetResult();
    }

    private void HandleTimeLeftChanged()
    {
        if (TimeLeft <= TimeSpan.Zero)
        {
            _ = HandleCountdownEnded();
        }
    }

    private async Task HandleCountdownEnded()
    {        
        CurrQuestionCount++;
        await GetNewQuestion();
    }

    
    private void HandleQuestion()
    {
        if (Question == null)
        {
            HandleLastQuestion();
            return;
        }

        Question = new Question
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

    private void HandleLastQuestion()
    {
        if (_finishedLast)
        {
            NavigateAndPop(new AfterGameViewModel(HostScreen));
        }
        else
        {
            NavigateAndPop(new FinishedEarlyViewModel(HostScreen, Room, _playersFinished)); 
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


    private Question? _question;

    public Question? Question
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
    
    
    /// <summary>
    /// Used for syncing the time with the Countdown thread
    /// </summary>
    private TimeSpan _internalTimeLeft;
    
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
