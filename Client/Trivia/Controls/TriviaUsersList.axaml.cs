using Avalonia;
using Avalonia.Controls;
using Avalonia.Media;

namespace Trivia.Controls;

public class TriviaUsersList : ContentControl
{
    public static readonly StyledProperty<IBrush?> PointerOverBrushProperty = AvaloniaProperty.Register<TriviaUsersList, IBrush?>(
        nameof(PointerOverBrush));

    public IBrush? PointerOverBrush
    {
        get => GetValue(PointerOverBrushProperty);
        set => SetValue(PointerOverBrushProperty, value);
    }

    public static readonly StyledProperty<IBrush?> PressedBrushProperty = AvaloniaProperty.Register<TriviaUsersList, IBrush?>(
        nameof(PressedBrush));

    public IBrush? PressedBrush
    {
        get => GetValue(PressedBrushProperty);
        set => SetValue(PressedBrushProperty, value);
    }
}