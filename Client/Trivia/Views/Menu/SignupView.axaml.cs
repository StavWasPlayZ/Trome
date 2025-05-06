using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class SignupView : PageViewControl<SignupViewModel>
{
    public SignupView()
    {
        InitializeComponent();
    }
}