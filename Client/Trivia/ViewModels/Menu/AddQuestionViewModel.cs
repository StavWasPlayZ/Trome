using System.Collections.Generic;
using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class AddQuestionViewModel : PageViewModel
{
    public ReactiveCommand<Unit, Unit> AddQuestionCommand { get; }
    private string? _prompt;
    public string? Prompt
    {
        get => _prompt;
        set => this.RaiseAndSetIfChanged(ref _prompt, value);
    }
    
    private List<string> _answers = [];
    public List<string> Answers
    {
        get => _answers;
        set => this.RaiseAndSetIfChanged(ref _answers, value);
    }
    
    public AddQuestionViewModel(IScreen hostScreen) : base(hostScreen)
    {
        AddQuestionCommand = ReactiveCommand.CreateFromTask(async _ =>
        {
            // TODO
        });
    }

    public AddQuestionViewModel()
    {
        AddQuestionCommand = NoOpCommand;
    }
}