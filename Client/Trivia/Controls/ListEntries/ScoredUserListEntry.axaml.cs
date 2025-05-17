using Avalonia;
using Avalonia.Controls.Primitives;

namespace Trivia.Controls.ListEntries;

public class ScoredUserListEntry : TemplatedControl
{
    public static readonly StyledProperty<int> NumberedPlaceProperty = AvaloniaProperty.Register<ScoredUserListEntry, int>(
        nameof(NumberedPlace));

    public int NumberedPlace
    {
        get => GetValue(NumberedPlaceProperty);
        set => SetValue(NumberedPlaceProperty, value);
    }
}