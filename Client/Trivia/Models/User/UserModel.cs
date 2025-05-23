namespace Trivia.Models.User;

public class UserModel : Raw.User
{
    public bool IsCurrentUser { get; init; }
}