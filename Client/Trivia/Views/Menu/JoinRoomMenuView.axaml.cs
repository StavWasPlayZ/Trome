using System;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Input;
using Avalonia.LogicalTree;
using Trivia.Models;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class JoinRoomMenuView : PageViewControl<JoinRoomMenuViewModel>
{
    public TimeSpan RoomInfoPanelAnimationTime { get; private set; } = TimeSpan.FromMilliseconds(200);
    private double _roomInfoPanelWidth;
    
    public JoinRoomMenuView()
    {
        InitializeComponent();        
    }

    private void OnAttachedToVisualTree(object? sender, VisualTreeAttachmentEventArgs e)
    {        
        _roomInfoPanelWidth = RoomInfoPanel.Width;
        // 20 is the padding we defaulted to.
        FixedWidthGrid.Width = _roomInfoPanelWidth - 20;

        if (!Design.IsDesignMode)
        {
            CloseRoomPanel();
        }
    }
    
    private void OnAttachedToLogicalTree(object? sender, LogicalTreeAttachmentEventArgs e)
    {
        ViewModel?.RunRefreshRoomsThread();
    }
    

    private void RoomListBox_OnPointerReleased(object? sender, PointerReleasedEventArgs pointerReleasedEventArgs)
    {
        var listbox = (sender as ListBox)!;
        
        if (listbox.SelectedItem == null)
        {
            CloseRoomPanel();
            return;
        }

        ExpandRoomPanel((listbox.SelectedItem as RoomModel)!);
    }
    
    private void ExpandRoomPanel(RoomModel roomModel)
    {
        RoomInfoPanel.Width = _roomInfoPanelWidth;
        ViewModel!.SelectedRoom = roomModel;
    }

    private void CloseRoomPanel()
    {
        RoomInfoPanel.Width = 0;
        ViewModel!.SelectedRoom = null;
    }
}