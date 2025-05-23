using Avalonia;
using Avalonia.Controls.Primitives;
using Trivia.Models.User;

namespace Trivia.Controls.ListEntries;

public class RoomUserListEntry : TemplatedControl
{
    public static readonly StyledProperty<RoomUserModel> UserProperty = AvaloniaProperty.Register<RoomUserListEntry, RoomUserModel>(
        nameof(RoomUserModel));

    public RoomUserModel User
    {
        get => GetValue(UserProperty);
        set => SetValue(UserProperty, value);
    }
}