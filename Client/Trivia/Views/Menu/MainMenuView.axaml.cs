using Avalonia.Controls;
using Avalonia.Interactivity;
using Trivia.ViewModels;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class MainMenuView : UserControl
{
    public MainMenuView()
    {
        InitializeComponent();
        
        PlayButton.Click += PlayButtonOnClick;
    }

    private void PlayButtonOnClick(object? sender, RoutedEventArgs e)
    {
        // ReSharper disable once UsePatternMatching
        var windowModel = (VisualRoot as MainWindow)?.DataContext as MainWindowViewModel;
        
        if (windowModel != null)
            windowModel.CurrentPage = new JoinMenuViewModel();
    }
}