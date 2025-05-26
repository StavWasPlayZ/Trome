using System.Collections.Generic;

namespace Trivia.Models.Raw;

public class Question
{
    public required string Prompt { get; init; }
    public required List<string> Answers { get; init; }
}