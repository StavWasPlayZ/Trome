using System;
using System.IO;
using System.Timers;
using Avalonia.Platform;
using NAudio.Vorbis;
using NAudio.Wave;
using NAudio.Wave.SampleProviders;

namespace Trivia.Services;

public class Track : IDisposable
{
    public static readonly TimeSpan TimeChangedCheckDelay = TimeSpan.FromMilliseconds(10);
    private static readonly TimeSpan TransitionTime = TimeSpan.FromSeconds(1.5);
    
    public SoundMeta Sound { get; }

    // Expose the volume sound provider as the "actual" sample provider.
    // So that the volume may be controlled.
    public ISampleProvider SampleProvider => _volumeSampleProvider!;
    
    public float Volume
    {
        get => _volumeSampleProvider!.Volume;
        set
        {
            _volumeSampleProvider!.Volume = value;
            TargetVolume = value;
        }
    }

    /// <summary>
    /// Linearly interpolates the current volume to this value.
    /// </summary>
    public float TargetVolume { get; set; }

    private VolumeSampleProvider? _volumeSampleProvider;
    private VorbisWaveReader? _oggReader;
    
    public event TimeChangedHandler? TimeChanged;
    private Timer? _timeChangedChecker;
    private TimeSpan? _oldTimeRead;
    
    public Track(SoundMeta sound)
    {
        Sound = sound;
        
        // Just immediately load it
        LoadTrack();
    }
    
    public bool IsMuted => Volume == 0;
    public void Mute()
    {
        Volume = 0f;
    }

    private void LoadTrack()
    {
        var uri = new Uri(Sound.Path);
        var fileStream = AssetLoader.Open(uri);
        
        if (fileStream == null)
            throw new FileNotFoundException($"Resource not found: {Sound.Path}");
        
        _oggReader = new VorbisWaveReader(fileStream);
        var sampleProvider = _oggReader.ToSampleProvider();
        
        _volumeSampleProvider = new VolumeSampleProvider(sampleProvider);
    }


    public void OnPlayed()
    {
        _timeChangedChecker = new Timer(TimeChangedCheckDelay);
        _timeChangedChecker.AutoReset = true;
        
        _timeChangedChecker.Elapsed += (_, _) =>
        {
            // Access the volume field directly to avoid changing TargetVolume too
            _volumeSampleProvider!.Volume = Lerp(Volume, TargetVolume, 1 / (float)TransitionTime.TotalSeconds / 100);
            
            var currTime = _oggReader!.CurrentTime;
            if (currTime == _oldTimeRead)
                return;
            
            TimeChanged?.Invoke(currTime);
            _oldTimeRead = currTime;
        };
        
        _timeChangedChecker.Start();
    }

    /// <summary>
    /// Linearly interpolates from a to b in t time.
    /// </summary>
    private static float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }
    
    
    public void Dispose()
    {
        _oggReader?.Dispose();
        _timeChangedChecker?.Dispose();

        TimeChanged = null;
        
        GC.SuppressFinalize(this);
    }
}

public delegate void TimeChangedHandler(TimeSpan time);
