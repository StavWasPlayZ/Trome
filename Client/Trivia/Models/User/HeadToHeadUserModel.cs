namespace Trivia.Models.User;

public record HeadToHeadUserModel : Raw.User
{
    public required int Points { get; init; }
    
    
    public static HeadToHeadUserModel FromUser(Raw.User user, int points)
    {
        return new HeadToHeadUserModel
        {
            Id = user.Id,
            Username = user.Username,
            Points = points
        };
    }
}