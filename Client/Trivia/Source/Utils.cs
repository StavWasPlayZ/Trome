using System;
using Avalonia.Controls;

namespace Trivia;

public static class Utils
{
    /// <summary>
    /// Triggers the given command if the selected item of the pressed list box was changed.
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
}