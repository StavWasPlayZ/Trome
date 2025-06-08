using System;
using LibVLCSharp.Shared;

namespace Trivia.Services;

public class MusicService : IDisposable
{
    public static MusicService Instance { get; } = new();
    private MusicService() { }
    
    private LibVLC? _libvlc;
    private MediaPlayer? _backgroundPlayer, _triviaPlayer;

    public void Initialize()
    {
        Core.Initialize();
        
        _libvlc = new LibVLC();
        _backgroundPlayer = new MediaPlayer(_libvlc);
        _triviaPlayer = new MediaPlayer(_libvlc);
    }

    public void Dispose()
    {
        _backgroundPlayer?.Dispose();
        _libvlc?.Dispose();
        _triviaPlayer?.Dispose();
        
        GC.SuppressFinalize(this);
    }
}
