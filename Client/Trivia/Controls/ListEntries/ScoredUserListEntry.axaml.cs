using Avalonia;
using Avalonia.Controls.Primitives;
using Trivia.Models.User;

namespace Trivia.Controls.ListEntries;

public class ScoredUserListEntry : TemplatedControl
{
    public static readonly StyledProperty<UserScoreModel?> ScoresProperty = AvaloniaProperty.Register<ScoredUserListEntry, UserScoreModel?>(
        nameof(Scores));

    public UserScoreModel? Scores
    {
        get => GetValue(ScoresProperty);
        set => SetValue(ScoresProperty, value);
    }
}