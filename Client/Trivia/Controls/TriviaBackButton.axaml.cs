using System.Windows.Input;
using Avalonia;
using Avalonia.Controls.Primitives;

namespace Trivia.Controls;

public class TriviaBackButton : TemplatedControl
{
    public static readonly StyledProperty<ICommand> CommandProperty = AvaloniaProperty.Register<TriviaBackButton, ICommand>(
        nameof(Command));

    public ICommand Command
    {
        get => GetValue(CommandProperty);
        set => SetValue(CommandProperty, value);
    }
}