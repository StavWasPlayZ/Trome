using Avalonia.ReactiveUI;
using Trivia.ViewModels.Popups;

namespace Trivia.Views.Popups;

public partial class StatsPopupView : ReactiveUserControl<StatsPopupViewModel>
{
    public StatsPopupView()
    {
        InitializeComponent();
    }
}