using System;
using System.Reactive;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;
using Trivia.Services;

namespace Trivia.ViewModels;

public abstract class PageViewModel(IScreen hostScreen) : ViewModelBase, IRoutableViewModel
{
    protected static readonly Communicator Comm = Communicator.Instance;
    
    public IScreen HostScreen { get; } = hostScreen;
    public string? UrlPathSegment { get; } = Guid.NewGuid().ToString()[..5];
    
    public ApplicationService AppService => App.AppService;


    public ReactiveCommand<Unit, IRoutableViewModel>? NavigateBackCommand { get; } =
        MainWindowViewModel?.Router.NavigateBack;


    protected static MainWindowViewModel? MainWindowViewModel =>
        (Application.Current?.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime)
            ?.MainWindow?.DataContext as MainWindowViewModel;

    protected static IObservable<IRoutableViewModel>? NavigateTo(PageViewModel pageViewModel)
    {
        return MainWindowViewModel?.Router.Navigate.Execute(pageViewModel);
    }
    protected static IObservable<IRoutableViewModel>? NavigateAndReset(PageViewModel pageViewModel)
    {
        return MainWindowViewModel?.Router.Navigate.Execute(pageViewModel);
    }

    protected static ReactiveCommand<Unit, IRoutableViewModel> NavigateReactiveCommand(Func<PageViewModel> pageViewModel)
    {
        return ReactiveCommand.CreateFromObservable(() => NavigateTo(pageViewModel())!);
    }
    protected static ReactiveCommand<Unit, IRoutableViewModel> NavigateAndResetReactiveCommand(Func<PageViewModel> pageViewModel)
    {
        return ReactiveCommand.CreateFromObservable(() => NavigateTo(pageViewModel())!);
    }
}