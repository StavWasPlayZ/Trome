using System.Collections.Generic;

namespace Trivia.Models.Raw;

public class Question
{
    public required string Prompt { get; init; }
    public required List<string> Answers { get; init; }

    
    public static Question MockQuestion = new Question
    {
        Prompt = "Lorem ipsum etc i dont remember it all but here is some multilined text",
        Answers = [
            "Answer 1",
            "Answer 2",
            "Answer 3",
            "Answer 4"
        ]
    };
}