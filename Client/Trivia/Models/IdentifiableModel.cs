namespace Trivia.Models;

public record IdentifiableModel
{
    public required int Id { get; init; }
    
    public virtual bool Equals(IdentifiableModel? other)
    {
        return Id == other?.Id;
    }

    public override int GetHashCode()
    {
        return Id.GetHashCode();
    }
}