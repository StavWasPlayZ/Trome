using Avalonia.Controls;
using Trivia.ViewModels;

namespace Trivia.Views;

public class ViewControl : UserControl
{
    protected void NavigateTo(PageViewModel pageViewModel)
    {
        // ReSharper disable once UsePatternMatching
        var windowModel = (VisualRoot as MainWindow)?.DataContext as MainWindowViewModel;
        
        if (windowModel != null)
            windowModel.CurrentPage = pageViewModel;
    }
}