using System.Reactive;
using ReactiveUI;
using Trivia.Controls.Popups;

namespace Trivia.ViewModels.Menu;

public class StatisticsViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
    public ReactiveCommand<Unit, Unit>? ShowStatsPopup { get; } = ReactiveCommand.Create(() =>
    {
        if (MainWindowViewModel == null)
            return;
        
        MainWindowViewModel.PopupContents = new StatsPopup
        {
            CloseCommand = MainWindowViewModel.CloseDialogCommand
        };
    });
}