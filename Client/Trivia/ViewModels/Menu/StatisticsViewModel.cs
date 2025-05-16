using System.Reactive;
using ReactiveUI;
using Trivia.Controls.Popup;

namespace Trivia.ViewModels.Menu;

public class StatisticsViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
    public ReactiveCommand<Unit, Unit>? ShowStatsPopup { get; } = ReactiveCommand.Create(() =>
    {
        var windowModel = GetMainWindowViewModel();
        if (windowModel == null)
            return;
        
        windowModel.PopupContents = new StatsPopup();
    });
}