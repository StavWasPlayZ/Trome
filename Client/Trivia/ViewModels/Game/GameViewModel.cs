using ReactiveUI;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class GameViewModel : PageViewModel
{
    public RoomData Data { get; }
    
    public GameViewModel(IScreen hostScreen, RoomData data) : base(hostScreen)
    {
        Data = data;
    }

    public GameViewModel()
    {
        Data = Room.CreateMockRoom(AppService.SessionUser!).Data;
        _leadingUsername = "Username";
        _question = Question.MockQuestion;
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
        set => this.RaiseAndSetIfChanged(ref _question, value);
    }


    private string? _leadingUsername;

    public string? LeadingUsername
    {
        get => _leadingUsername;
        set => this.RaiseAndSetIfChanged(ref _leadingUsername, value);
    }
}
