using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class LoginView : PageViewControl<LoginViewModel>
{
    public LoginView()
    {
        InitializeComponent();
    }
}