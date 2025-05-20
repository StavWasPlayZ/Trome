using System;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Input;
using ReactiveUI;
using Trivia.Models;
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
        var listbox = (sender as ListBox)!;
        
        if (listbox.SelectedItem == null)
        {
            CloseRoomPanel();
            return;
        }

        ExpandRoomPanel((listbox.SelectedItem as Room)!);
    }
    
    private void ExpandRoomPanel(Room room)
    {
        RoomInfoPanel.Width = _roomInfoPanelWidth;
        ViewModel!.SelectedRoom = room;
    }

    private void CloseRoomPanel()
    {
        RoomInfoPanel.Width = 0;
        ViewModel!.SelectedRoom = null;
    }
}