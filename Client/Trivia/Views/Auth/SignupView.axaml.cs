using ReactiveUI;
using Trivia.ViewModels.Auth;

namespace Trivia.Views.Auth;

public partial class SignupView : PageViewControl<SignupViewModel>
{
    public SignupView()
    {
        InitializeComponent();
        
        this.WhenActivated(_ => { });
    }
}