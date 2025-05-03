using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class JoinMenuView : PageViewControl<JoinMenuViewModel>
{
    public JoinMenuView()
    {
        this.WhenActivated(_ => { });
        InitializeComponent();
    }
}