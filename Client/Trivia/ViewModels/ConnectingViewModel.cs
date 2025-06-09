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
            if (!InitMusicService())
                return;
            
            Communicator.Instance.Connect()
                .ToObservable()
                .Subscribe(
                    _ => OnConnectionEstablished(),
                    OnConnectionFailed
                )
                .DisposeWith(disposables);
        });
    }

    private bool InitMusicService()
    {
        try
        {
            App.MusicService.Initialize();
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            MusicServiceFailed = true;
            return false;
        }
        
        App.MusicService.LoadTracks();
        
        return true;
    }

    public ConnectingViewModel() { }
    
    
    private bool _musicServiceFailed;

    public bool MusicServiceFailed
    {
        get => _musicServiceFailed;
        set => this.RaiseAndSetIfChanged(ref _musicServiceFailed, value);
    }
    
    private bool _connectionFailed;

    public bool ConnectionFailed
    {
        get => _connectionFailed;
        set => this.RaiseAndSetIfChanged(ref _connectionFailed, value);
    }

    private void OnConnectionEstablished()
    {
        App.MusicService.Play();
        App.MusicService.PlayBackgroundTrack();
        
        Dispatcher.UIThread.Post(() => NavigateAndReset(new LoginViewModel(HostScreen)));
    }

    private void OnConnectionFailed(Exception e)
    {
        Console.Error.WriteLine(e);
        Dispatcher.UIThread.Post(() => ConnectionFailed = true);
    }
}