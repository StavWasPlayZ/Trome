using System;
using System.Reactive.Disposables;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class SettingsMenuViewModel : PageViewModel
{
    public bool MusicServiceAvailable => App.MusicService != null;
    
    public SettingsMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        _volume = App.MusicService?.MasterVolume ?? 0f;
        
        this.WhenActivated(disposables =>
        {
            this
                .WhenAnyValue(x => x.Volume)
                .Subscribe(_ => App.MusicService!.MasterVolume = Volume)
                .DisposeWith(disposables);
        });
    }

    public SettingsMenuViewModel()
    {
        _volume = 0.5f;
    }


    private float _volume;
    public float Volume
    {
        get => _volume;
        set => this.RaiseAndSetIfChanged(ref _volume, value);
    }
}