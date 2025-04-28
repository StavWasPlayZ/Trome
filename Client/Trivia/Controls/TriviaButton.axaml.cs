using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Primitives;

namespace Trivia.Controls;

public class TriviaButton : TemplatedControl
{
    public static readonly StyledProperty<string> TextProperty =
        AvaloniaProperty.Register<TriviaButton, string>(nameof(Text));
    
    public string Text
    {
        get => GetValue(TextProperty);
        set => SetValue(TextProperty, value);
    }
}