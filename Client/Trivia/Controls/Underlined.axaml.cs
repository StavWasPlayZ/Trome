using Avalonia;
using Avalonia.Controls;

namespace Trivia.Controls;

public class Underlined : ContentControl
{
    public static readonly StyledProperty<double> UnderlineThicknessProperty = AvaloniaProperty.Register<Underlined, double>(
        nameof(UnderlineThickness));

    public double UnderlineThickness
    {
        get => GetValue(UnderlineThicknessProperty);
        set => SetValue(UnderlineThicknessProperty, value);
    }
}