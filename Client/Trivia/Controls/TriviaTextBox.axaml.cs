using Avalonia;
using Avalonia.Controls;
using Avalonia.Media;

namespace Trivia.Controls;

public class TriviaTextBox : ContentControl
{
    public static readonly StyledProperty<IBrush?> UnderlineBrushProperty = AvaloniaProperty.Register<TriviaTextBox, IBrush?>(
        nameof(UnderlineBrush));

    public IBrush? UnderlineBrush
    {
        get => GetValue(UnderlineBrushProperty);
        set => SetValue(UnderlineBrushProperty, value);
    }
}