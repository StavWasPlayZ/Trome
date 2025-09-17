using Avalonia.ReactiveUI;
using Trivia.ViewModels.Popups;

namespace Trivia.Views.Popups;

public partial class GuestStatsPopupView : ReactiveUserControl<GuestStatsPopupViewModel>
{
    public GuestStatsPopupView()
    {
        InitializeComponent();
    }
}