using System.Windows.Input;
using Avalonia;
using Avalonia.Controls.Primitives;
using Avalonia.Media;

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

    public static readonly StyledProperty<ISolidColorBrush> IconColorProperty = AvaloniaProperty.Register<TriviaButton, ISolidColorBrush>(
        nameof(IconColor));

    public ISolidColorBrush IconColor
    {
        get => GetValue(IconColorProperty);
        set => SetValue(IconColorProperty, value);
    }
    

    public static readonly StyledProperty<IBrush> BorderBrushSelectedProperty = AvaloniaProperty.Register<TriviaButton, IBrush>(
        nameof(BorderBrushSelected));

    public IBrush BorderBrushSelected
    {
        get => GetValue(BorderBrushSelectedProperty);
        set => SetValue(BorderBrushSelectedProperty, value);
    }

    public static readonly StyledProperty<IBrush> BorderBrushPressedProperty = AvaloniaProperty.Register<TriviaButton, IBrush>(
        nameof(BorderBrushPressed));

    public IBrush BorderBrushPressed
    {
        get => GetValue(BorderBrushPressedProperty);
        set => SetValue(BorderBrushPressedProperty, value);
    }

    public static readonly StyledProperty<Thickness> BorderThicknessSelectedProperty = AvaloniaProperty.Register<TriviaButton, Thickness>(
        nameof(BorderThicknessSelected));

    public Thickness BorderThicknessSelected
    {
        get => GetValue(BorderThicknessSelectedProperty);
        set => SetValue(BorderThicknessSelectedProperty, value);
    }

    public static readonly StyledProperty<IBrush> BackgroundSelectedProperty = AvaloniaProperty.Register<TriviaButton, IBrush>(
        nameof(BackgroundSelected));

    public IBrush BackgroundSelected
    {
        get => GetValue(BackgroundSelectedProperty);
        set => SetValue(BackgroundSelectedProperty, value);
    }


    public static readonly StyledProperty<ICommand> CommandProperty = AvaloniaProperty.Register<TriviaButton, ICommand>(
        nameof(Command));

    public ICommand Command
    {
        get => GetValue(CommandProperty);
        set => SetValue(CommandProperty, value);
    }
}