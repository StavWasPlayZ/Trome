using System;
using System.ComponentModel.DataAnnotations;
using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class SignupViewModel : AuthViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> ToLoginCommand { get; }
    
    public SignupViewModel(IScreen hostScreen) : base(hostScreen)
    {
        ToLoginCommand = NavigateAndResetReactiveCommand(
            () => new LoginViewModel(hostScreen)
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


    protected override void UpdateMayAuthenticate()
    {
        MayAuthenticate =
            !string.IsNullOrEmpty(Username)
            && !string.IsNullOrEmpty(Email)
            && !string.IsNullOrEmpty(Password)
            && !string.IsNullOrEmpty(RepPassword)
            && Password == RepPassword
            && !string.IsNullOrEmpty(Phone)
        ;
    }
    
    
    private string? _username;

    [Required]
    public string? Username
    {
        get => _username;
        set => this.RaiseAndSetIfChanged(ref _username, value);
    }
    
    private string? _email;

    [Required]
    [EmailAddress]
    public string? Email
    {
        get => _email;
        set => this.RaiseAndSetIfChanged(ref _email, value);
    }
    
    private string? _password;

    [Required]
    public string? Password
    {
        get => _password;
        set => this.RaiseAndSetIfChanged(ref _password, value);
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
