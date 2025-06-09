using System;
using System.IO;
using Avalonia.Platform;
using NAudio.Vorbis;
using NAudio.Wave;
using NAudio.Wave.SampleProviders;

namespace Trivia.Services;

public class Track : IDisposable
{
    public SoundMeta Sound { get; }

    // Expose the volume sound provider as the "actual" sample provider.
    // So that the volume may be controlled.
    public ISampleProvider SampleProvider => _volumeSampleProvider!;
    
    public float Volume
    {
        get => _volumeSampleProvider!.Volume;
        set => _volumeSampleProvider!.Volume = value;
    }

    private VolumeSampleProvider? _volumeSampleProvider;

    private VorbisWaveReader? _oggReader;
    
    public Track(SoundMeta sound)
    {
        Sound = sound;
        
        // Just immediately load it
        LoadTrack();
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

    public void Dispose()
    {
        _oggReader?.Dispose();
        GC.SuppressFinalize(this);
    }
}