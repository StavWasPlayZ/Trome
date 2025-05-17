using System;
using System.ComponentModel.DataAnnotations;
using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Auth;

public class LoginViewModel : AuthViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> ToSignupCommand { get; }
    
    public LoginViewModel(IScreen hostScreen) : base(hostScreen)
    {
        ToSignupCommand = NavigateAndResetReactiveCommand(
            () => new SignupViewModel(hostScreen)
        );
        
        this
            .WhenAnyValue(
                vm => vm.Username,
                vm => vm.Password
            )
            .Subscribe(_ => UpdateMayAuthenticate());
    }
    

    protected override void UpdateMayAuthenticate()
    {
        MayAuthenticate =
            !string.IsNullOrEmpty(Username)
            && !string.IsNullOrEmpty(Password);
    }
    
    
    private string? _username;

    [Required]
    public string? Username
    {
        get => _username;
        set => this.RaiseAndSetIfChanged(ref _username, value);
    }
    
    private string? _password;

    [Required]
    public string? Password
    {
        get => _password;
        set => this.RaiseAndSetIfChanged(ref _password, value);
    }
    
}