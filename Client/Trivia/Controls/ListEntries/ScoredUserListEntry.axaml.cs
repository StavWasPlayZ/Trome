using Avalonia;
using Avalonia.Controls.Primitives;
using Trivia.Models.User;

namespace Trivia.Controls.ListEntries;

public class ScoredUserListEntry : TemplatedControl
{
    public static readonly StyledProperty<UserScoreModel> ScoresProperty = AvaloniaProperty.Register<ScoredUserListEntry, UserScoreModel>(
        nameof(Scores));

    public UserScoreModel Scores
    {
        get => GetValue(ScoresProperty);
        set => SetValue(ScoresProperty, value);
    }


    public static readonly StyledProperty<bool> IsEmptySlotProperty = AvaloniaProperty.Register<ScoredUserListEntry, bool>(
        nameof(IsEmptySlot));

    public bool IsEmptySlot
    {
        get => GetValue(IsEmptySlotProperty);
        set => SetValue(IsEmptySlotProperty, value);
    }
}