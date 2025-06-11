using Avalonia;
using Avalonia.Controls.Primitives;
using Trivia.Models;

namespace Trivia.Controls.ListEntries;

public class AfterGameListEntry : TemplatedControl
{
    public static readonly StyledProperty<PlayerResultModel> ResultProperty =
        AvaloniaProperty.Register<AfterGameListEntry, PlayerResultModel>(nameof(Result));

    public PlayerResultModel Result
    {
        get => GetValue(ResultProperty);
        set => SetValue(ResultProperty, value);
    }

    public static readonly StyledProperty<RoomModel> RoomModelProperty =
        AvaloniaProperty.Register<AfterGameListEntry, RoomModel>(nameof(RoomModel));

    public RoomModel RoomModel
    {
        get => GetValue(RoomModelProperty);
        set => SetValue(RoomModelProperty, value);
    }
}
