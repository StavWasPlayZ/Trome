using System;
using Trivia.Services;

namespace Trivia.Source;

public static class MusicTracks
{
    public static readonly SoundMeta Background = new(
        "avares://Trivia/Assets/Sounds/BGM/background.ogg",
        TimeSpan.FromMilliseconds(51_200)
    );
    public static  readonly SoundMeta TriviaStress = new(
        "avares://Trivia/Assets/Sounds/BGM/trivia_stress.ogg",
        TimeSpan.FromMilliseconds(51_200)
    );
    
    public static readonly SoundMeta[] AllTracks =
    [
        Background,
        TriviaStress
    ];
}
