using System;
using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;

namespace Trivia.ViewModels.Auth;

public class LoginViewModel : AuthViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> ToSignupCommand { get; }
    public ReactiveCommand<Unit, Unit> LoginCommand { get; }
    
    public LoginViewModel(IScreen hostScreen) : base(hostScreen)
    {
        ToSignupCommand = NavigateAndResetReactiveCommand(
            () => new SignupViewModel(hostScreen)
        );

        LoginCommand = ReactiveCommand.Create(
            DoLogin,
            this.WhenAnyValue(vm => vm.MayAuthenticate)
        );
        
        this
            .WhenAnyValue(
                vm => vm.Username,
                vm => vm.Password
            )
            .Subscribe(_ => UpdateMayAuthenticate());
    }
    
    public LoginViewModel()
    {
        ToSignupCommand = NoOpNavCommand;
        LoginCommand = NoOpCommand;
    }

    
    private void DoLogin()
    {
        Comm.SendRequest<LoginResponse>(
            new LoginRequest(Username!, Password!),
            HandleAuthResponse,
            HandleErrorResponse
        );
    }


    protected override void UpdateMayAuthenticate()
    {
        MayAuthenticate =
            !string.IsNullOrEmpty(Username)
            && !string.IsNullOrEmpty(Password);
    }
    
}