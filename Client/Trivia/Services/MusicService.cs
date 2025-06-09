using System;
using System.Timers;
using NAudio.Wave;
using NAudio.Wave.SampleProviders;
using Trivia.Source;

namespace Trivia.Services;

public class MusicService : IDisposable
{
    private const float MasterVolume = .3f;

    private static readonly TimeSpan PlayLatency = TimeSpan.FromMilliseconds(100);
    
    public static MusicService Instance { get; } = new();
    private MusicService() { }

    private WaveOutEvent? _outputDevice;
    private MixingSampleProvider? _soundMixer;
    
    private VolumeSampleProvider? _masterVolumeProvider;
    private Track? _backgroundTrack;
    private Track? _triviaTrack;

    private Timer? _repeatTimer;

    public void Initialize()
    {
        // NAudio unfortunately only works under Windows.
        // The other approach would be to use native OpenAL calls (OpenTK), but that is
        // the most cancerous activity I've yet to have done in this project.
        if (!OperatingSystem.IsWindows())
            return;
        
        _outputDevice = new WaveOutEvent
        {
            DesiredLatency = (int) PlayLatency.TotalMilliseconds,
        };

        // 44.1 kHz is the CD audio standard.
        // 2 channels = stereo (which our track does support).
        _soundMixer = new MixingSampleProvider(WaveFormat.CreateIeeeFloatWaveFormat(44100, 2));
        
        _masterVolumeProvider = new VolumeSampleProvider(_soundMixer)
        {
            Volume = MasterVolume
        };

        _outputDevice.Init(_masterVolumeProvider);
    }


    private void LoadTrack(Track track)
    {
        // Mute the track by default
        track.Mute();
        
        _soundMixer!.AddMixerInput(track.SampleProvider);
    }

    public void LoadTracks()
    {
        LoadBackgroundTrack();
        LoadTriviaTrack();
    }
    
    private Track LoadBackgroundTrack()
    {
        _backgroundTrack = new Track(MusicTracks.Background);
        LoadTrack(_backgroundTrack);
        
        return _backgroundTrack;
    }
    private Track LoadTriviaTrack()
    {
        _triviaTrack = new Track(MusicTracks.TriviaStress);
        LoadTrack(_triviaTrack);
        
        return _triviaTrack;
    }
    
    public void PlayBackgroundTrack()
    {
        _triviaTrack!.Mute();
        _backgroundTrack!.Volume = 1f;

        _masterVolumeProvider!.Volume = MasterVolume * .75f;
    }
    public void PlayTriviaTrack()
    {
        _backgroundTrack!.Mute();
        _triviaTrack!.Volume = 1f;
        
        _masterVolumeProvider!.Volume = MasterVolume;
    }

    
    public void Play()
    {
        _outputDevice!.Play();

        // Start the track repeater timer
        _repeatTimer = new Timer(MusicTracks.GlobalDuration - PlayLatency);
        _repeatTimer.AutoReset = true;
        _repeatTimer.Elapsed += OnTracksFinished;
        
        _repeatTimer.Start();
    }

    private void OnTracksFinished(object? sender, ElapsedEventArgs elapsedEventArgs)
    {
        ReAddTrack(_backgroundTrack!, LoadBackgroundTrack);
        ReAddTrack(_triviaTrack!, LoadTriviaTrack);
        
        _repeatTimer!.Interval = MusicTracks.GlobalDuration.TotalMilliseconds;
    }

    private static void ReAddTrack(Track track, Func<Track> trackLoader)
    {
        // Note that we're not repeating the mixer, but adding new instances.
        // This is because there is a loop fading built into the tracks.
        // It will otherwise sound choppy.
        var newTrack = trackLoader();
        
        // We do not need to dispose of the old track, because the underlying stream resources are shared anyways.
        // As such, they are already disposed in our Dispose method.
        // ty avalonia xoxo <3
        
        newTrack.Volume = track.Volume;
    }
    

    public void Dispose()
    {
        _outputDevice?.Stop();
        _outputDevice?.Dispose();
        
        _backgroundTrack?.Dispose();
        _triviaTrack?.Dispose();
        
        _repeatTimer?.Dispose();
        
        GC.SuppressFinalize(this);
    }
}
