namespace Trivia.Models.User;

public record RoomUserModel : UserModel
{
    public bool IsAdmin { get; init; }


    public static RoomUserModel FromUserModel(UserModel userModel, bool isAdmin)
    {
        return new RoomUserModel
        {
            Id = userModel.Id,
            Username = userModel.Username,
            IsCurrentUser = userModel.IsCurrentUser,
            IsAdmin = isAdmin
        };
    }
    
    public static RoomUserModel FromUser(Raw.User user, bool isCurrentUser, bool isAdmin)
    {
        return FromUserModel(FromUser(user, isCurrentUser), isAdmin);
    }
}
