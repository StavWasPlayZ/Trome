using System;
using NAudio.Wave;
using NAudio.Wave.SampleProviders;
using Trivia.Source;

namespace Trivia.Services;

public class MusicService : IDisposable
{
    public static MusicService Instance { get; } = new();
    private MusicService() { }

    private WaveOutEvent? _outputDevice;
    private MixingSampleProvider? _soundMixer;
    
    private VolumeSampleProvider? _masterVolumeProvider;
    private Track? _backgroundTrack;
    private Track? _triviaTrack;

    public void Initialize()
    {
        // NAudio unfortunately only works under Windows.
        // The other approach would be to use native OpenAL calls (OpenTK), but that is
        // the most cancerous activity I've yet to have done in this project.
        if (!OperatingSystem.IsWindows())
            return;
        
        _outputDevice = new WaveOutEvent();

        // 44.1 kHz is the CD audio standard.
        // 2 channels = stereo (which our track does support).
        _soundMixer = new MixingSampleProvider(WaveFormat.CreateIeeeFloatWaveFormat(44100, 2))
        {
            // Don't remove tracks when done (they're looping)
            ReadFully = true
        };
        
        _masterVolumeProvider = new VolumeSampleProvider(_soundMixer);
        _outputDevice.Init(_masterVolumeProvider);
    }


    private void LoadTrack(Track track)
    {
        // Mute the track by default
        track.Volume = 0f;
        
        _soundMixer!.AddMixerInput(track.SampleProvider);
    }

    public void LoadTracks()
    {
        LoadBackgroundTrack();
        LoadTriviaTrack();
    }
    
    private void LoadBackgroundTrack(SoundMeta? soundMeta = null)
    {
        _backgroundTrack = new Track(soundMeta ?? MusicTracks.Background);
        LoadTrack(_backgroundTrack);
    }
    private void LoadTriviaTrack(SoundMeta? soundMeta = null)
    {
        _triviaTrack = new Track(soundMeta ?? MusicTracks.TriviaStress);
        LoadTrack(_triviaTrack);
    }
    
    public void PlayBackgroundTrack()
    {
        _backgroundTrack!.Volume = 1f;
    }
    public void PlayTriviaTrack()
    {
        _triviaTrack!.Volume = 1f;
    }

    public void Play()
    {
        _outputDevice!.Play();
    }
    

    public void Dispose()
    {
        _outputDevice?.Stop();
        _outputDevice?.Dispose();
        
        _backgroundTrack?.Dispose();
        _triviaTrack?.Dispose();
        
        GC.SuppressFinalize(this);
    }
}
