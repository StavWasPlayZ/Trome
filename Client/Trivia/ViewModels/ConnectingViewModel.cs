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
            InitMusicService();
            
            Communicator.Instance.Connect()
                .ToObservable()
                .Subscribe(
                    _ => OnConnectionEstablished(),
                    OnConnectionFailed
                )
                .DisposeWith(disposables);
        });
    }

    private void InitMusicService()
    {
        bool succeed;
        
        try
        {
            succeed = App.MusicService!.Initialize();
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            succeed = false;
            
        }

        if (!succeed)
        {
            Console.Error.WriteLine("Music service not initialized");
            App.SetMusicServiceUnavailable();
        }
    }

    public ConnectingViewModel() { }
    
    private bool _connectionFailed;

    public bool ConnectionFailed
    {
        get => _connectionFailed;
        set => this.RaiseAndSetIfChanged(ref _connectionFailed, value);
    }

    private void OnConnectionEstablished()
    {
        App.MusicService?.Play();
        App.MusicService?.PlayBackgroundTrack(false);
        
        Dispatcher.UIThread.Post(() => NavigateAndReset(new LoginViewModel(HostScreen)));
    }

    private void OnConnectionFailed(Exception e)
    {
        Console.Error.WriteLine(e);
        Dispatcher.UIThread.Post(() => ConnectionFailed = true);
    }
}