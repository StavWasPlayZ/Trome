using System.Reactive;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;

namespace Trivia.ViewModels.Menu;

public class TestViewModel(IScreen hostScreen) : PageViewModel(hostScreen)
{
}