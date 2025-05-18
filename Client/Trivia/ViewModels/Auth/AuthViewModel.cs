using System;
using System.ComponentModel.DataAnnotations;
using ReactiveUI;
using Trivia.Codec.S2C.Response;
using Trivia.Codec.S2C.Response.Status;
using Trivia.Models.User;
using Trivia.ViewModels.Menu;

namespace Trivia.ViewModels.Auth;

public abstract class AuthViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
    protected void HandleAuthResponse<TStatus>(RegistrationResponse<TStatus> response, bool succeed) where TStatus : Enum
    {
        if (!succeed)
        {
            ErrorMessage = StatusTranscriber.Transcribe(response);
            return;
        }

        App.AppService.SessionedUser = new User((int)response.UserId!, Username!);
        
        NavigateAndReset(new MainMenuViewModel(HostScreen));
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


    //NOTE: We may do client-side validations here.
    // But meh.
    protected abstract void UpdateMayAuthenticate();
}