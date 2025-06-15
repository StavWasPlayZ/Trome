using System;
using System.Net;
using System.Reactive;
using System.Reactive.Linq;
using ReactiveUI;

namespace Trivia.ViewModels;

public class ServerIpViewModel : PageViewModel
{
    public ReactiveCommand<Unit, IRoutableViewModel> ConnectCommand { get; }
    
    public ServerIpViewModel(IScreen hostScreen) : base(hostScreen)
    {
        ConnectCommand = ReactiveCommand.CreateFromObservable(() =>
            NavigateTo(new ConnectingViewModel(
                HostScreen,
                new IPEndPoint(IPAddress.Parse(_serverIp!), int.Parse(_serverPort!))
            ))!,
            
            this
                .WhenAnyValue(
                    x => x.ServerIp,
                    x => x.ServerPort
                )
                .Select(_ => ValidateFields())
        );
    }

    public ServerIpViewModel()
    {
        ConnectCommand = NoOpNavCommand;
    }


    private bool ValidateFields()
    {
        if (string.IsNullOrWhiteSpace(_serverIp!))
        {
            return false;
        }

        if (!int.TryParse(ServerPort!, out var serverPort))
        {
            return false;
        }

        if (serverPort is < 1024 or > 49151)
        {
            ErrorMessage = "Server port must be within the valid range (1024 - 49151)";
            return false;
        }

        try
        {
            IPAddress.Parse(_serverIp!);
        }
        catch (FormatException)
        {
            ErrorMessage = "Invalid IP address";
            return false;
        }

        ErrorMessage = null;
        return true;
    }
    
    
    private string? _serverIp = "127.0.0.1";

    public string? ServerIp
    {
        get => _serverIp;
        set => this.RaiseAndSetIfChanged(ref _serverIp, value);
    }
    
    
    private string? _serverPort = "6942";

    public string? ServerPort
    {
        get => _serverPort;
        set => this.RaiseAndSetIfChanged(ref _serverPort, value);
    }
    
    
    private string? _errorMessage;

    public string? ErrorMessage
    {
        get => _errorMessage;
        set => this.RaiseAndSetIfChanged(ref _errorMessage, value);
    }
}