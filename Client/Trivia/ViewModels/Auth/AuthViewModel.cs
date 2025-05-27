using System.ComponentModel.DataAnnotations;
using ReactiveUI;
using Trivia.Codec.S2C.Response;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;
using Trivia.ViewModels.Menu;

namespace Trivia.ViewModels.Auth;

public abstract class AuthViewModel : PageViewModel
{
    protected AuthViewModel(IScreen hostScreen) : base(hostScreen) { }
    protected AuthViewModel()
    {
        // Just to see the final button design
        MayAuthenticate = true;
    }
    
    
    protected void HandleAuthResponse(RegistrationResponse response)
    {
        App.AppService.SessionUser = new User
        {
            Id = response.UserId,
            Username = Username!
        };
        NavigateAndReset(new MainMenuViewModel(HostScreen));
    }
    
    protected void HandleErrorResponse(ErrorResponse response)
    {
        ErrorMessage = response.Transcribe();
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
    
    
    private string? _errorMessage;

    public string? ErrorMessage
    {
        get => _errorMessage;
        protected set => this.RaiseAndSetIfChanged(ref _errorMessage, value);
    }


    private bool _mayAuthenticate;

    protected bool MayAuthenticate
    {
        get => _mayAuthenticate;
        set => this.RaiseAndSetIfChanged(ref _mayAuthenticate, value);
    }
}