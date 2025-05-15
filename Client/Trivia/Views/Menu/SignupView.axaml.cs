using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class SignupView : PageViewControl<SignupViewModel>
{
    public SignupView()
    {
        InitializeComponent();
        
        this.WhenActivated(_ => { });
    }
}