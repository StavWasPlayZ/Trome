using System;
using System.IO;
using Avalonia.Platform;
using LibVLCSharp.Shared;

namespace Trivia.Services;

public class Track(LibVLC libvlc) : IDisposable
{
    private MediaPlayer Player { get; } = new(libvlc);

    private Media? Media
    {
        get => Player.Media;
        set => Player.Media = value;
    }


    private string? _tempFilePath;

    public void LoadSound(SoundMeta soundMeta)
    {
        _tempFilePath = Path.GetTempFileName();
    
        using (var assetStream = AssetLoader.Open(new Uri(soundMeta.Path)))
        {
            using var fileStream = File.OpenWrite(_tempFilePath);
            assetStream.CopyTo(fileStream);
        }
    
        Media = new Media(libvlc, _tempFilePath);
    }

    public void Play()
    {
        Player.Play();
    }

    public void Dispose()
    {
        Player.Dispose();
        Media?.Dispose();

        if (_tempFilePath != null)
        {
            File.Delete(_tempFilePath);
        }
        
        GC.SuppressFinalize(this);
    }
}
