using System;
using Trivia.Services;

namespace Trivia.Source;

public static class MusicTracks
{
    public static readonly TimeSpan GlobalDuration = TimeSpan.FromSeconds(51.15);
    
    public static readonly SoundMeta Background = new(
        "avares://Trivia/Assets/Sounds/BGM/background.ogg",
        GlobalDuration
    );
    public static readonly SoundMeta TriviaStress = new(
        "avares://Trivia/Assets/Sounds/BGM/trivia_stress.ogg",
        GlobalDuration
    );
    
    public static readonly SoundMeta[] AllTracks =
    [
        Background,
        TriviaStress
    ];
}
