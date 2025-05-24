using System;
using System.Reactive.Disposables;
using System.Reactive.Threading.Tasks;
using Avalonia.Threading;
using ReactiveUI;
using Trivia.ViewModels.Auth;

namespace Trivia.ViewModels;

public class ConnectingViewModel : PageViewModel
{
    public ConnectingViewModel(IScreen hostScreen) : base(hostScreen)
    {
        this.WhenActivated(disposables =>
        {
            Communicator.Instance.Connect()
                .ToObservable()
                .Subscribe(
                    _ => OnConnectionEstablished(),
                    OnConnectionFailed
                )
                .DisposeWith(disposables);
        });
    }
    
    public ConnectingViewModel() { }
    
    
    private bool _connectionFailed;

    public bool ConnectionFailed
    {
        get => _connectionFailed;
        set => this.RaiseAndSetIfChanged(ref _connectionFailed, value);
    }

    private void OnConnectionEstablished() =>
        Dispatcher.UIThread.Post(() => NavigateAndReset(new LoginViewModel(HostScreen)));
    
    private void OnConnectionFailed(Exception e)
    {
        Console.Error.WriteLine(e);
        Dispatcher.UIThread.Post(() => ConnectionFailed = true);
    }
}