using System.Collections.ObjectModel;
using System.Linq;
using System.Reactive;
using System.Reactive.Linq;
using System.Threading;
using System.Threading.Tasks;
using DynamicData;
using DynamicData.Binding;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;

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

    public ObservableCollection<string> Answers { get; } = new(
        Enumerable.Range(0, 4)
            .Select(_ => "")
    );
    
    public AddQuestionViewModel(IScreen hostScreen) : base(hostScreen)
    {
        var answersNotEmpty = Answers
            .ToObservableChangeSet()
            .ToCollection()
            .Select(answers => !answers.Any(string.IsNullOrWhiteSpace));
        
        var isPromptNotEmpty = this.WhenAnyValue(x => x.Prompt)
            .Select(prompt => !string.IsNullOrWhiteSpace(prompt));
        
        AddQuestionCommand = ReactiveCommand.CreateFromTask(
            SendAddQuestionRequest,
            
            isPromptNotEmpty.CombineLatest(
                answersNotEmpty,
                (a, b) => a && b
            )
        );
    }

    private async Task SendAddQuestionRequest(CancellationToken _)
    {
        await Comm.SendRequestAsync<AddQuestionResponse>(
            new AddQuestionRequest(_prompt!, Answers)
        );

        NavigateBackCommand!.Execute();
    }

    public AddQuestionViewModel()
    {
        AddQuestionCommand = NoOpCommand;
    }
}