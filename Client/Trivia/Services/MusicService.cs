using System;
using LibVLCSharp.Shared;

namespace Trivia.Services;

public class MusicService : IDisposable
{
    public static MusicService Instance { get; } = new();
    private MusicService() { }
    
    private LibVLC? _libvlc;

    private Track? _backgroundTrack, _triviaTrack;

    public void Initialize()
    {
        Core.Initialize();
        
        _libvlc = new LibVLC();
        _backgroundTrack = new Track(_libvlc);
        _triviaTrack = new Track(_libvlc);
    }
    
    
    public void LoadBackgroundTrack(SoundMeta? soundMeta = null)
    {
        _backgroundTrack?.LoadSound(soundMeta ?? MusicTracks.Background);
    }
    public void LoadTriviaTrack(SoundMeta? soundMeta = null)
    {
        _triviaTrack?.LoadSound(soundMeta ?? MusicTracks.TriviaStress);
    }
    
    public void PlayBackgroundTrack()
    {
        _backgroundTrack?.Play();
    }
    public void PlayTriviaTrack()
    {
        _triviaTrack?.Play();
    }
    

    public void Dispose()
    {
        _libvlc?.Dispose();
        
        _backgroundTrack?.Dispose();
        _triviaTrack?.Dispose();
        
        GC.SuppressFinalize(this);
    }
}
