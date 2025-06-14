using Avalonia.Controls;
using Avalonia.Input;
using Trivia.Models.User;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class StatisticsView : PageViewControl<StatisticsViewModel>
{
    public StatisticsView()
    {
        InitializeComponent();
    }

    private void SelectingItemsControl_OnPointerReleased(object? sender, PointerReleasedEventArgs pointerReleasedEventArgs)
    {
        var listbox = (sender as ListBox)!;
        
        if (listbox.SelectedItem == null)
            return;

        var scores = (listbox.SelectedItem as UserScoreModel)!.Scores;
        if (scores is null)
            return;
        
        StatisticsViewModel.ShowUserStatsPopup(scores.User);
        
        listbox.SelectedItem = null;
    }
}
