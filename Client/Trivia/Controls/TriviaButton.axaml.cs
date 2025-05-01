using Avalonia;
using Avalonia.Controls.Primitives;

namespace Trivia.Controls;

public class TriviaButton : TemplatedControl
{
    public static readonly StyledProperty<string> TextProperty = AvaloniaProperty.Register<TriviaButton, string>(
        nameof(Text));
    
    public string Text
    {
        get => GetValue(TextProperty);
        set => SetValue(TextProperty, value);
    }

    public static readonly StyledProperty<string?> IconSvgPathProperty = AvaloniaProperty.Register<TriviaButton, string?>(
        nameof(IconSvgPath));

    public string? IconSvgPath
    {
        get => GetValue(IconSvgPathProperty);
        set => SetValue(IconSvgPathProperty, value);
    }

    public static readonly StyledProperty<Thickness> IconMarginProperty = AvaloniaProperty.Register<TriviaButton, Thickness>(
        nameof(IconMargin));

    public Thickness IconMargin
    {
        get => GetValue(IconMarginProperty);
        set => SetValue(IconMarginProperty, value);
    }

    public static readonly StyledProperty<bool> IsIconButtonProperty = AvaloniaProperty.Register<TriviaButton, bool>(
        nameof(IsIconButton));

    /// <summary>
    /// Determines whether this button will not display text, but rather only
    /// the icon button.
    /// </summary>
    public bool IsIconButton
    {
        get => GetValue(IsIconButtonProperty);
        set => SetValue(IsIconButtonProperty, value);
    }
}