namespace Trivia.Models.User;

public class UserModel : Raw.User
{
    public bool IsCurrentUser { get; init; }
    
    
    public static UserModel FromUser(Raw.User user, bool isCurrentUser)
    {
        return new UserModel
        {
            Id = user.Id,
            Username = user.Username,
            IsCurrentUser = isCurrentUser
        };
    }
}