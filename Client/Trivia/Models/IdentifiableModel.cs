namespace Trivia.Models;

public class IdentifiableModel
{
    public required int Id { get; init; }

    public override bool Equals(object? obj)
    {
        return obj is IdentifiableModel model
               && Id == model.Id;
    }
    
    public static bool operator==(IdentifiableModel left, IdentifiableModel right) => left.Equals(right);
    public static bool operator!=(IdentifiableModel left, IdentifiableModel right) => !(left == right);

    public override int GetHashCode()
    {
        return Id;
    }
}