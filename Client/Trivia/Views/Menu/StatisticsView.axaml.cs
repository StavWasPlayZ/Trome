using System;
using Avalonia.Controls;
using Avalonia.ReactiveUI;
using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class StatisticsView : ReactiveUserControl<StatisticsViewModel>
{
    public StatisticsView()
    {
        InitializeComponent();

        this.WhenActivated(_ => { });
    }

    private void SelectingItemsControl_OnSelectionChanged(object? sender, SelectionChangedEventArgs e)
    {
        if (e.AddedItems.Count != 1)
            return;
        
        ViewModel!.ShowStatsPopup?.Execute().Subscribe();
        
        (sender as ListBox)!.SelectedItem = null;
    }
}