using System;
using System.Reactive.Disposables;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Threading;
using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class JoinMenuView : PageViewControl<JoinMenuViewModel>
{
    public TimeSpan RoomInfoPanelAnimationTime { get; private set; } = TimeSpan.FromMilliseconds(200);
    private double _roomInfoPanelWidth;
    
    public JoinMenuView()
    {
        this.WhenActivated(disposables =>
        {
            Disposable
                .Create(() => RoomListBox.SelectionChanged -= RoomListBoxOnSelectionChanged)
                .DisposeWith(disposables);
        });
        InitializeComponent();
        
        AttachedToVisualTree += OnAttachedToVisualTree;
        
        RoomListBox.SelectionChanged += RoomListBoxOnSelectionChanged;
    }

    private void OnAttachedToVisualTree(object? sender, VisualTreeAttachmentEventArgs e)
    {
        _roomInfoPanelWidth = RoomInfoPanel.Width;
        RoomInfoPanel.Width = 0;
    }

    private void RoomListBoxOnSelectionChanged(object? sender, SelectionChangedEventArgs e)
    {
        //TODO: Implement selected room
        
        if (e.AddedItems.Count == 0)
        {
            RoomInfoPanel.Width = 0;
            return;
        }

        // If we not only removed, but also added, we swapped.
        // Animate the animation in and out.
        if (e.RemovedItems.Count == 1)
        {
            RoomInfoPanel.Width = 0;

            Task.Run(async () =>
            {
                RoomInfoPanelAnimationTime /= 1.5;
                await Task.Delay(RoomInfoPanelAnimationTime);
                RoomInfoPanelAnimationTime *= 1.5;

                Dispatcher.UIThread.Post(() => RoomInfoPanel.Width = _roomInfoPanelWidth);
            });
        }
        else
        {
            RoomInfoPanel.Width = _roomInfoPanelWidth;
        }
    }
}