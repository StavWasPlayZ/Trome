using System.Windows.Input;
using Avalonia;
using Avalonia.Controls.Primitives;

namespace Trivia.Controls.Popups;

public class StatsPopup : TemplatedControl
{
    public static readonly StyledProperty<ICommand?> CloseCommandProperty = AvaloniaProperty.Register<StatsPopup, ICommand?>(
        nameof(CloseCommand));

    public ICommand? CloseCommand
    {
        get => GetValue(CloseCommandProperty);
        set => SetValue(CloseCommandProperty, value);
    }
}