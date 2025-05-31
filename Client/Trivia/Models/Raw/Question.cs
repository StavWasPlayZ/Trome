using System.Collections.Generic;

namespace Trivia.Models.Raw;

public class Question
{
    public required string Prompt { get; init; }
    public required List<string> Answers { get; init; }

    
    public static readonly Question MockQuestion = new()
    {
        Prompt = "Lorem ipsum etc i dont remember it all but here is some multilined text",
        Answers = [
            "Answer 1",
            "Very Multilined Answer 2",
            "Looooooooooooooooooooooooong Answer 3",
            "Answer 4"
        ]
    };
}