using System.Reactive;
using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.ViewModels;

public abstract class AuthViewModel : PageViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> AuthenticateCommand { get; }
    
    protected AuthViewModel(IScreen hostScreen) : base(hostScreen)
    {
        AuthenticateCommand = ReactiveCommand.CreateFromObservable(
            () => NavigateTo(new MainMenuViewModel(hostScreen))!,
            this.WhenAnyValue(vm => vm.MayAuthenticate)
        );
    }
    

    private bool _mayAuthenticate;

    protected bool MayAuthenticate
    {
        get => _mayAuthenticate;
        set => this.RaiseAndSetIfChanged(ref _mayAuthenticate, value);
    }


    //NOTE: We may do client-side validations here.
    // But meh.
    protected abstract void UpdateMayAuthenticate();
}