using System;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;

namespace Trivia.ViewModels;

public class PageViewModel(IScreen hostScreen) : ViewModelBase, IRoutableViewModel
{
    public IScreen HostScreen { get; } = hostScreen;
    public string? UrlPathSegment { get; } = Guid.NewGuid().ToString()[..5];

    protected static MainWindowViewModel? GetMainWindowViewModel()
    {
        return (Application.Current?.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime)
            ?.MainWindow?.DataContext as MainWindowViewModel;
    }
    
    protected static IObservable<IRoutableViewModel>? NavigateTo(PageViewModel pageViewModel)
    {
        return GetMainWindowViewModel()?.Router.Navigate.Execute(pageViewModel);
    }
}