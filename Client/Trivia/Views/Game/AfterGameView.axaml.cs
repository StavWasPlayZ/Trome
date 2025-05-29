using System;
using Avalonia.Controls;
using Avalonia.VisualTree;
using Trivia.ViewModels.Game;

namespace Trivia.Views.Game;

public partial class AfterGameView : PageViewControl<AfterGameViewModel>
{
    public AfterGameView()
    {
        InitializeComponent();
    }

    private void OnLayoutUpdated(object? sender, EventArgs e)
    {
        HalfHeight.Height = (this.GetVisualRoot() as Window)!.Bounds.Height / 2;
    }
}