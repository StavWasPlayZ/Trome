namespace Trivia.Models.Raw;

public record User : IdentifiableModel
{
    public required string Username { get; init; }
}
