using System;
using ReactiveUI;
using Trivia.Codec.S2C.Response;
using Trivia.Codec.S2C.Response.Status;
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
        
        //TODO: Assign global user
        
        NavigateAndReset(new MainMenuViewModel(HostScreen));
    }
    
    
    private string? _errorMessage;

    public string? ErrorMessage
    {
        get => this._errorMessage;
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