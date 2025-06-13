using System;
using System.Reactive;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class SettingsMenuViewModel : PageViewModel
{
    public bool MusicServiceAvailable => App.MusicService != null;
    
    public ReactiveCommand<Unit, IRoutableViewModel> AddQuestionCommand { get; }
    
    public SettingsMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        _volume = App.MusicService?.MasterVolume ?? 0f;

        AddQuestionCommand = NavigateReactiveCommand(() => new AddQuestionViewModel(HostScreen));
        
        this.WhenActivated(disposables =>
        {
            this
                .WhenAnyValue(x => x.Volume)
                .Skip(1)
                .Subscribe(_ => App.MusicService!.MasterVolume = Volume)
                .DisposeWith(disposables);
        });
    }

    public SettingsMenuViewModel()
    {
        AddQuestionCommand = NoOpNavCommand;
        _volume = 0.5f;
    }


    private float _volume;
    public float Volume
    {
        get => _volume;
        set => this.RaiseAndSetIfChanged(ref _volume, value);
    }
}