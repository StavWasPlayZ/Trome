using System.Windows.Input;
using Avalonia;
using Avalonia.Controls;

namespace Trivia.Controls;

public class MenuPanelExpanded : ContentControl
{
    public static readonly StyledProperty<string> TitleProperty = AvaloniaProperty.Register<MenuPanelExpanded, string>(
        nameof(Title));

    public string Title
    {
        get => GetValue(TitleProperty);
        set => SetValue(TitleProperty, value);
    }
    
    
    public static readonly StyledProperty<ICommand> BackButtonCommandProperty = AvaloniaProperty.Register<MenuPanelExpanded, ICommand>(
        nameof(BackButtonCommand));

    public ICommand BackButtonCommand
    {
        get => GetValue(BackButtonCommandProperty);
        set => SetValue(BackButtonCommandProperty, value);
    }
}