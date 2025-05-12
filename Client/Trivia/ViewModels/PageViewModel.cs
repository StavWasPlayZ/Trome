using System;
using System.Reactive;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;

namespace Trivia.ViewModels;

public abstract class PageViewModel(IScreen hostScreen) : ViewModelBase, IRoutableViewModel
{
    public IScreen HostScreen { get; } = hostScreen;
    public string? UrlPathSegment { get; } = Guid.NewGuid().ToString()[..5];
    
    
    public ReactiveCommand<Unit, IRoutableViewModel>? NavigateBackCommand { get; } =
        GetMainWindowViewModel()?.Router.NavigateBack;
    

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