using Avalonia;
using Avalonia.Controls.Primitives;
using Trivia.Models;

namespace Trivia.Controls.ListEntries;

public class UserListEntry : TemplatedControl
{
    public static readonly StyledProperty<User> UserProperty = AvaloniaProperty.Register<UserListEntry, User>(
        nameof(User));

    public User User
    {
        get => GetValue(UserProperty);
        set => SetValue(UserProperty, value);
    }
}