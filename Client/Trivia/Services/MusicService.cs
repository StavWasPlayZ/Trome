using LibVLCSharp.Shared;

namespace Trivia.Services;

public class MusicService
{
    public static MusicService Instance { get; } = new();
    private MusicService() { }

    public static void Initialize()
    {
        Core.Initialize();
    }
}
