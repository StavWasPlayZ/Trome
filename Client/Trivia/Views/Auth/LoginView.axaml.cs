using ReactiveUI;
using Trivia.ViewModels.Auth;

namespace Trivia.Views.Auth;

public partial class LoginView : PageViewControl<LoginViewModel>
{
    public LoginView()
    {
        InitializeComponent();
        
        this.WhenActivated(_ => { });
    }
}