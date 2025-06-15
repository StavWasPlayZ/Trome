using System;
using System.IO;
using Avalonia.Controls;
using Avalonia.Platform;
using Trivia.ViewModels;

namespace Trivia;

public static class Utils
{
    public static MainWindowViewModel? MainWindowViewModelInstance =>
        MainWindow.Instance?.DataContext as MainWindowViewModel;
    
    /// <summary>
    /// Triggers the given command if the selected item of a ListBox was changed.
    /// Resets its value back to null when done.
    /// </summary>
    /// <typeparam name="T">The list type</typeparam>
    public static void PerformListTriggerAction<T>(object? sender, Action<T> triggeredAction)
    {
        var listbox = (sender as ListBox)!;
        if (listbox.SelectedItem == null)
            return;

        var selectedItem = (T) listbox.SelectedItem;
        
        triggeredAction(selectedItem);
        
        listbox.SelectedItem = null;
    }


    public static string ReadAvares(Uri uri)
    {
        using var stream = AssetLoader.Open(uri);
        using var reader = new StreamReader(stream);
        return reader.ReadToEnd();
    }
    
    public static byte[] ReadAvaresBytes(Uri uri)
    {
        if (!AssetLoader.Exists(uri))
        {
            throw new FileNotFoundException($"\"{uri.AbsolutePath}\" not found");
        }
        
        using var stream = AssetLoader.Open(uri);
        using var memStream = new MemoryStream();
        
        stream.CopyTo(memStream);
        return memStream.ToArray();
    }
}