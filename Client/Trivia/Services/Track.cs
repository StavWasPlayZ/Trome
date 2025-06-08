using System;
using System.IO;
using Avalonia.Platform;
using LibVLCSharp.Shared;

namespace Trivia.Services;

public class Track(LibVLC libvlc) : IDisposable
{
    private MediaPlayer Player { get; } = new(libvlc);
    
    public SoundMeta CurrentTrack { get; private set; }


    private string? _tempFilePath;

    public void LoadSound(SoundMeta soundMeta)
    {
        _tempFilePath = Path.GetTempFileName();
    
        using (var assetStream = AssetLoader.Open(new Uri(soundMeta.Path)))
        {
            using var fileStream = File.OpenWrite(_tempFilePath);
            assetStream.CopyTo(fileStream);
        }
    
        Player.Media = new Media(libvlc, _tempFilePath);
        CurrentTrack = soundMeta;
    }

    public void Play()
    {
        Player.Play();

        Player.TimeChanged += (sender, args) =>
        {
            Player.Volume = 100;
            Player.Mute = false;
        };
    }

    public void Dispose()
    {
        Player.Dispose();

        if (_tempFilePath != null)
        {
            File.Delete(_tempFilePath);
        }
        
        GC.SuppressFinalize(this);
    }
}
