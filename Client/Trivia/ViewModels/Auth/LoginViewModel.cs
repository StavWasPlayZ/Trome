using System;
using System.ComponentModel.DataAnnotations;
using System.Reactive;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C.Response;
using Trivia.Codec.S2C.Response.Status;
using Trivia.ViewModels.Menu;

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
                await DoSignIn();
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

    private async Task DoSignIn()
    {
        var task = new TaskCompletionSource();
        
        Comm.ProtocolResponseReceived += OnProtocolResponseReceived;
        Comm.SendRequest(new LoginRequest(Username, Password));
        
        await task.Task;
        return;

        void OnProtocolResponseReceived(IProtocolResponse response)
        {
            if (response is not LoginResponse loginResponse)
                return;
            
            HandleLoginInResponse(loginResponse);
            
            Comm.ProtocolResponseReceived -= OnProtocolResponseReceived;
            task.TrySetResult();
        }
    }

    private void HandleLoginInResponse(LoginResponse response)
    {
        if (response.Status != LoginStatus.Success)
        {
            ErrorMessage = StatusTranscriber.Transcribe(response);
            return;
        }
        
        //TODO: Assign global user
        
        NavigateAndReset(new MainMenuViewModel(HostScreen));
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