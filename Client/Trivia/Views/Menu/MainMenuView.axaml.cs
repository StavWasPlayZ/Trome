using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class MainMenuView : PageViewControl<MainMenuViewModel>
{
    public MainMenuView()
    {
        this.WhenActivated(_ => { });
        
        InitializeComponent();
    }
}