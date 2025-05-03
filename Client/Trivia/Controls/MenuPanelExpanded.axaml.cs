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
    
    
    public static readonly StyledProperty<ICommand> CommandProperty = AvaloniaProperty.Register<MenuPanelExpanded, ICommand>(
        nameof(Command));

    public ICommand Command
    {
        get => GetValue(CommandProperty);
        set => SetValue(CommandProperty, value);
    }
}