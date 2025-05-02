using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class MainMenuView : ViewControl
{
    public MainMenuView()
    {
        InitializeComponent();
        
        PlayButton.Click += (_, _) => NavigateTo(new JoinMenuViewModel());
    }
}