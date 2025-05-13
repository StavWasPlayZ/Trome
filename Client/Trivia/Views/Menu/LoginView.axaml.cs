using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class LoginView : PageViewControl<LoginViewModel>
{
    public LoginView()
    {
        this.WhenActivated(_ => { });
        
        InitializeComponent();
    }
}