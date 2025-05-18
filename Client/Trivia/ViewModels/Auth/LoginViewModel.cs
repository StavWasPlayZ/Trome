using System;
using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C.Response;
using Trivia.Codec.S2C.Response.Status;

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

        LoginCommand = ReactiveCommand.CreateFromTask(
            async () =>
            {
                HandleLoginResponse(
                    await Comm.SendRequestAwaitResponse<LoginResponse>(new LoginRequest(Username!, Password!))
                );
            },
            this.WhenAnyValue(vm => vm.MayAuthenticate)
        );
        
        this
            .WhenAnyValue(
                vm => vm.Username,
                vm => vm.Password
            )
            .Subscribe(_ => UpdateMayAuthenticate());
    }

    private void HandleLoginResponse(LoginResponse response)
    {
        HandleAuthResponse(response, response.Status == LoginStatus.Success);
    }


    protected override void UpdateMayAuthenticate()
    {
        MayAuthenticate =
            !string.IsNullOrEmpty(Username)
            && !string.IsNullOrEmpty(Password);
    }
    
}