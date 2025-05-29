using System;
using System.Reactive;
using System.Reactive.Disposables;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Services;

namespace Trivia.ViewModels;

public abstract class PageViewModel : ViewModelBase, IRoutableViewModel, IActivatableViewModel
{
    public ViewModelActivator Activator { get; } = new();

    protected static readonly Communicator Comm = Communicator.Instance;
    
    public IScreen HostScreen { get; }
    public string? UrlPathSegment { get; } = Guid.NewGuid().ToString()[..5];
    
    public ApplicationService AppService { get; }
    
    
    protected PageViewModel(IScreen hostScreen)
    {
        AppService = App.AppService;
        HostScreen = hostScreen;
        
        this.WhenActivated(disposables =>
        {
            Disposable
                .Create(() => Comm.PacketReceived -= CommOnPacketReceived)
                .DisposeWith(disposables);
        });
    }

    
    public void SubToServerEvents()
    {
        Comm.PacketReceived += CommOnPacketReceived;
    }

    protected virtual void CommOnPacketReceived(IS2CPacket packet) { }
    
    
    // Mock implementations
    protected PageViewModel()
    {
        AppService = ApplicationService.MockAppService;
        HostScreen = null!;
    }

    protected static ReactiveCommand<Unit, Unit> NoOpCommand { get; } =
        ReactiveCommand.Create(() => { });
    protected static ReactiveCommand<Unit, IRoutableViewModel> NoOpNavCommand { get; } =
        ReactiveCommand.Create(IRoutableViewModel () => null!);


    public ReactiveCommand<Unit, IRoutableViewModel>? NavigateBackCommand { get; } =
        MainWindowViewModel?.Router.NavigateBack;


    protected static MainWindowViewModel? MainWindowViewModel =>
        MainWindow.Instance?.DataContext as MainWindowViewModel;

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