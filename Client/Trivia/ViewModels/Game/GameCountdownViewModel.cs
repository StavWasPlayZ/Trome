using System;
using System.Reactive.Disposables;
using Avalonia.Threading;
using ReactiveUI;

namespace Trivia.ViewModels.Game;

public class GameCountdownViewModel : PageViewModel
{
    private DispatcherTimer? _countdownTimer;

    public GameCountdownViewModel(IScreen hostScreen) : base(hostScreen)
    {
        _countdownTimer = new DispatcherTimer
        {
            Interval = TimeSpan.FromSeconds(1)
        };
        
        this.WhenActivated(disposables =>
        {
            _countdownTimer.Tick += CountdownTimerOnTick;
            _countdownTimer.Start();
            
            Disposable
                .Create(() =>
                {
                    _countdownTimer.Stop();
                    _countdownTimer.Tick -= CountdownTimerOnTick;
                })
                .DisposeWith(disposables);
        });
    }

    public GameCountdownViewModel()
    {}
    
    
    private void CountdownTimerOnTick(object? sender, EventArgs e)
    {
        Countdown--;

        if (Countdown == 0)
        {
            NavigateAndPop(new GameViewModel());
        }
    }

    
    private int _countdown = 5;

    public int Countdown
    {
        get => _countdown;
        set => this.RaiseAndSetIfChanged(ref _countdown, value);
    }
}
