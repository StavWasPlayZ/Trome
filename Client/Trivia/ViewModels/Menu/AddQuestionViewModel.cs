using System.Collections.Generic;
using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;

namespace Trivia.ViewModels.Menu;

public class AddQuestionViewModel : PageViewModel
{
    public ReactiveCommand<Unit, Unit> AddQuestionCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel>? NavigateBackCommand { get; }
    private string _prompt = "";
    public string? Prompt
    {
        get => _prompt;
        set => this.RaiseAndSetIfChanged(ref _prompt, value);
    }
    
    private List<string> _answers = ["", "", "", ""];
    public List<string> Answers
    {
        get => _answers;
        set => this.RaiseAndSetIfChanged(ref _answers, value);
    }
    
    public AddQuestionViewModel(IScreen hostScreen) : base(hostScreen)
    {
        AddQuestionCommand = ReactiveCommand.CreateFromTask(async _ =>
        {
            if(_prompt != null)
            {
                await Comm.SendRequestAwaitResponse<AddQuestionResponse>(
                    new AddQuestionRequest(_prompt, _answers)
                );
                NavigateReactiveCommand(
                    () => new MainMenuViewModel(hostScreen)
                );
            }
        });
        
        NavigateBackCommand = NavigateReactiveCommand(
                () => new MainMenuViewModel(hostScreen)
        );
    }

    public AddQuestionViewModel()
    {
        AddQuestionCommand = NoOpCommand;
        NavigateBackCommand = NoOpNavCommand;
    }
}