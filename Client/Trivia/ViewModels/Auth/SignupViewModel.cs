using System;
using System.ComponentModel.DataAnnotations;
using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;

namespace Trivia.ViewModels.Auth;

public class SignupViewModel : AuthViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> ToLoginCommand { get; }
    public ReactiveCommand<Unit, Unit> SignupCommand { get; }
    
    public SignupViewModel(IScreen hostScreen) : base(hostScreen)
    {
        ToLoginCommand = NavigateAndResetReactiveCommand(
            () => new LoginViewModel(hostScreen)
        );
        
        SignupCommand = ReactiveCommand.Create(
            DoSignup,
            this.WhenAnyValue(vm => vm.MayAuthenticate)
        );

        this
            .WhenAnyValue(
                vm => vm.Username,
                vm => vm.Email,
                vm => vm.Password,
                vm => vm.RepPassword,
                vm => vm.Phone,
                vm => vm.Address
            )
            .Subscribe(_ => UpdateMayAuthenticate());
    }

    public SignupViewModel()
    {
        ToLoginCommand = NoOpNavCommand;
        SignupCommand = NoOpCommand;
    }
    

    private void DoSignup()
    {
        Comm.SendRequest<LoginResponse>(
            new SignupRequest(
                Username!,
                Password!,
                Email!,
                Phone!.Replace(" ", ""),
                //TODO: Re-add Address and Birthdate fields; Add scroller in View.
                Address,
                "17/06/2008"
            ),
            
            HandleAuthResponse,
            HandleErrorResponse
        );
    }


    protected override void UpdateMayAuthenticate()
    {
        DoPasswordsMatch = Password == RepPassword;
        
        MayAuthenticate =
            !string.IsNullOrEmpty(Username)
            && !string.IsNullOrEmpty(Email)
            && !string.IsNullOrEmpty(Password)
            && !string.IsNullOrEmpty(RepPassword)
            && DoPasswordsMatch
            && !string.IsNullOrEmpty(Phone)
        ;
    }


    private bool _doPasswordsMatch;

    public bool DoPasswordsMatch
    {
        get => _doPasswordsMatch;
        private set => this.RaiseAndSetIfChanged(ref _doPasswordsMatch, value);
    }
    
    
    private string? _email;

    [Required]
    [EmailAddress]
    public string? Email
    {
        get => _email;
        set => this.RaiseAndSetIfChanged(ref _email, value);
    }
    
    private string? _repPassword;

    [Required]
    public string? RepPassword
    {
        get => _repPassword;
        set => this.RaiseAndSetIfChanged(ref _repPassword, value);
    }
    
    private string? _phone;

    [Required]
    [Phone]
    public string? Phone
    {
        get => _phone;
        set => this.RaiseAndSetIfChanged(ref _phone, value);
    }
    
    private string? _address;

    public string? Address
    {
        get => _address;
        set => this.RaiseAndSetIfChanged(ref _address, value);
    }
    
}
