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
        Utils.PerformListTriggerAction<UserScoreModel>(sender, OnUserSelected);
    }

    private static void OnUserSelected(UserScoreModel userScoreModel)
    {
        if (userScoreModel.Scores is null)
            return;

        StatisticsViewModel.ShowUserStatsPopup(userScoreModel.Scores.User);
    }
}
