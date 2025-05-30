using Avalonia;
using Avalonia.Controls.Primitives;

namespace Trivia.Controls;

public class LeadUserDock : TemplatedControl
{
    public static readonly StyledProperty<string?> UsernameProperty = AvaloniaProperty.Register<LeadUserDock, string?>(
        nameof(Username));

    public string? Username
    {
        get => GetValue(UsernameProperty);
        set => SetValue(UsernameProperty, value);
    }
}