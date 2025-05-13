using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class LoginViewModel(IScreen hostScreen) : AuthViewModel(hostScreen)
{
}