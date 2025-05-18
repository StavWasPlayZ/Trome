using System.Collections.Generic;

namespace Trivia.Models;

public enum RoomStatus : uint
{
    Waiting = 0,
    Playing = 1,
    NotFound = 2
}

public record Room(
        RoomStatus GameStatus,
        int QuestionsCount,
        int SecsPerQuestion,
        List<LoggedUser> Players,
        bool IsGameRunning
    );