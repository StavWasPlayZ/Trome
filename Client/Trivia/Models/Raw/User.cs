namespace Trivia.Models.Raw;

public record User : IdentifiableModel
{
    public required string Username { get; init; }


    public static User CreateMock(int id)
    {
        return new User
        {
            Id = id,
            Username = $"User {id}"
        };
    }
}
