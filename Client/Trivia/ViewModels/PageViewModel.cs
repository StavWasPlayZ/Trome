using System;
using System.Reactive;
using System.Reactive.Disposables;
using System.Reactive.Linq;
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

    protected static RoutingState? Router => MainWindowViewModel?.Router;
    
    
    protected PageViewModel(IScreen hostScreen)
    {
        AppService = App.AppService;
        HostScreen = hostScreen;
        
        this.WhenActivated(disposables =>
        {
            Comm.PacketReceived += CommOnPacketReceived;

            Disposable
                .Create(() => Comm.PacketReceived -= CommOnPacketReceived)
                .DisposeWith(disposables);
        });
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
        Router?.NavigateBack;


    protected static MainWindowViewModel? MainWindowViewModel =>
        MainWindow.Instance?.DataContext as MainWindowViewModel;

    protected static IObservable<IRoutableViewModel>? NavigateTo(PageViewModel pageViewModel)
    {
        return Router?.Navigate.Execute(pageViewModel);
    }
    protected static IObservable<IRoutableViewModel>? NavigateAndReset(PageViewModel pageViewModel)
    {
        return Router?.NavigateAndReset.Execute(pageViewModel);
    }
    protected static IObservable<IRoutableViewModel>? NavigateAndPop(PageViewModel pageViewModel)
    {
        return Router?.NavigateBack
            .Execute()
            .SelectMany(_ => NavigateTo(pageViewModel)!);
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