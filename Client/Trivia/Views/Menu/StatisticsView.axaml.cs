using Avalonia.ReactiveUI;
using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class StatisticsView : ReactiveUserControl<StatisticsViewModel>
{
    public StatisticsView()
    {
        InitializeComponent();

        this.WhenActivated(_ => { });
    }
}