using System;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Input;
using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class JoinMenuView : PageViewControl<JoinMenuViewModel>
{
    public TimeSpan RoomInfoPanelAnimationTime { get; private set; } = TimeSpan.FromMilliseconds(200);
    private double _roomInfoPanelWidth;
    
    public JoinMenuView()
    {
        InitializeComponent();
        
        this.WhenActivated(_ => { });
    }

    private void OnAttachedToVisualTree(object? sender, VisualTreeAttachmentEventArgs e)
    {
        _roomInfoPanelWidth = RoomInfoPanel.Width;

        if (!Design.IsDesignMode)
        {    
            CloseRoomPanel();
        }
    }

    private void RoomListBox_OnPointerReleased(object? sender, PointerReleasedEventArgs pointerReleasedEventArgs)
    {
        //TODO: Implement selected room
        var listbox = (sender as ListBox)!;
        
        if (listbox.SelectedItem == null)
        {
            CloseRoomPanel();
            return;
        }

        // // If we not only removed, but also added, we swapped.
        // // Animate the animation in and out.
        // if (e.RemovedItems.Count == 1)
        // {
        //     CloseRoomPanel();
        //
        //     Task.Run(async () =>
        //     {
        //         RoomInfoPanelAnimationTime /= 1.5;
        //         await Task.Delay(RoomInfoPanelAnimationTime);
        //         RoomInfoPanelAnimationTime *= 1.5;
        //
        //         Dispatcher.UIThread.Post(ExpandRoomPanel);
        //     });
        //     
        //     return;
        // }

        ExpandRoomPanel();
    }
    
    private void ExpandRoomPanel() => RoomInfoPanel.Width = _roomInfoPanelWidth;
    private void CloseRoomPanel() => RoomInfoPanel.Width = 0;
}