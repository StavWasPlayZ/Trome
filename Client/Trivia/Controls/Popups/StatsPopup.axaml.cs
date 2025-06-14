using System.Windows.Input;
using Avalonia;
using Avalonia.Controls.Primitives;
using Trivia.Models.Raw;

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

    public static readonly StyledProperty<UserStatistics> StatsProperty = AvaloniaProperty.Register<StatsPopup, UserStatistics>(
        nameof(Stats));

    public UserStatistics Stats
    {
        get => GetValue(StatsProperty);
        set => SetValue(StatsProperty, value);
    }

    public static readonly StyledProperty<string> UsernameProperty = AvaloniaProperty.Register<StatsPopup, string>(
        nameof(Username));

    public string Username
    {
        get => GetValue(UsernameProperty);
        set => SetValue(UsernameProperty, value);
    }
}
