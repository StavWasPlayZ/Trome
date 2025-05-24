using System;
using Avalonia.Controls;
using Avalonia.Input;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class StatisticsView : PageViewControl<StatisticsViewModel>
{
    public StatisticsView()
    {
        InitializeComponent();
    }

    private void SelectingItemsControl_OnPointerReleased(object? sender, PointerReleasedEventArgs pointerReleasedEventArgs)
    {
        var listbox = (sender as ListBox)!;
        
        if (listbox.SelectedItem == null)
            return;
        
        ViewModel?.ShowStatsPopup?.Execute().Subscribe();
        
        listbox.SelectedItem = null;
    }
}