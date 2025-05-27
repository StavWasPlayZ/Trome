using Avalonia;
using Avalonia.Controls.Primitives;
using Trivia.Models.Raw;

namespace Trivia.Controls.ListEntries;

public class RoomUserListEntry : TemplatedControl
{
    public static readonly StyledProperty<User?> UserProperty = AvaloniaProperty.Register<RoomUserListEntry, User?>(
        nameof(User));

    public User? User
    {
        get => GetValue(UserProperty);
        set => SetValue(UserProperty, value);
    }

    public static readonly StyledProperty<bool> IsAdminProperty = AvaloniaProperty.Register<RoomUserListEntry, bool>(
        nameof(IsAdmin));

    public bool IsAdmin
    {
        get => GetValue(IsAdminProperty);
        set => SetValue(IsAdminProperty, value);
    }

    public static readonly StyledProperty<bool> IsCurrentUserProperty = AvaloniaProperty.Register<RoomUserListEntry, bool>(
        nameof(IsCurrentUser));

    public bool IsCurrentUser
    {
        get => GetValue(IsCurrentUserProperty);
        set => SetValue(IsCurrentUserProperty, value);
    }

    public static readonly StyledProperty<bool> IsEmptySlotProperty = AvaloniaProperty.Register<RoomUserListEntry, bool>(
        nameof(IsEmptySlot));

    public bool IsEmptySlot
    {
        get => GetValue(IsEmptySlotProperty);
        set => SetValue(IsEmptySlotProperty, value);
    }
}