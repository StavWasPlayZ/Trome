using System.Reactive;
using System.Reactive.Disposables;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class GameViewModel : PageViewModel
{
    public RoomData Data { get; }
    public ReactiveCommand<int, Unit> SubmitAnswerCommand { get; }
    
    public GameViewModel(IScreen hostScreen, RoomData data) : base(hostScreen)
    {
        Data = data;

        SubmitAnswerCommand = ReactiveCommand.CreateFromTask<int>(async (btnIndex, _) =>
            await SubmitAnswer(btnIndex)
        );
        
        this.WhenActivated(disposables =>
        {
            GetNewQuestion()
                .DisposeWith(disposables);
        });
    }

    public GameViewModel()
    {
        Data = Room.CreateMockRoom(AppService.SessionUser!).Data;
        _leadingUsername = "Username";
        _question = Question.MockQuestion;

        SubmitAnswerCommand = ReactiveCommand.Create<int>(_ => { });
    }


    private async Task GetNewQuestion()
    {
         var response = await Comm.SendRequestAwaitResponse<GetQuestionResponse>(new GetQuestionRequest());
         Question = response.Question;
         
         HandleQuestion();
    }

    private async Task SubmitAnswer(int btnIndex)
    {
        var response = await Comm.SendRequestAwaitResponse<SubmitAnswerResponse>(new SubmitAnswerRequest(btnIndex));
        Question = response.NewQuestion;
        
        HandleQuestion();
    }

    
    private void HandleQuestion()
    {
        if (Question == null)
        {
            HandleLastQuestion();
        }
    }

    private void HandleLastQuestion()
    {
        //TODO: Implement
    }
    
    
    private int _currQuestion = 1;

    public int CurrQuestion
    {
        get => _currQuestion;
        set => this.RaiseAndSetIfChanged(ref _currQuestion, value);
    }


    private Question? _question;

    public Question? Question
    {
        get => _question;
        private set => this.RaiseAndSetIfChanged(ref _question, value);
    }


    private string? _leadingUsername;

    public string? LeadingUsername
    {
        get => _leadingUsername;
        set => this.RaiseAndSetIfChanged(ref _leadingUsername, value);
    }
}
