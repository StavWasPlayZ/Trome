using System;
using System.Reactive;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using ReactiveUI;
using Trivia.Codec.S2C;

namespace Trivia.ViewModels;

public abstract class PageViewModel : ViewModelBase, IRoutableViewModel
{
    public IScreen HostScreen { get; }
    public string? UrlPathSegment { get; } = Guid.NewGuid().ToString()[..5];
    

    protected static RoutingState? Router => MainWindowViewModel?.Router;
    
    
    protected PageViewModel(IScreen hostScreen) : base(false)
    {
        HostScreen = hostScreen;
        
        this.WhenActivated(disposables =>
        {
            Comm.PacketReceived += CommOnPacketReceived;

            Disposable
                .Create(() => Comm.PacketReceived -= CommOnPacketReceived)
                .DisposeWith(disposables);
        });
    }
    
    protected PageViewModel() : base(true)
    {
        HostScreen = null!;
    }

    
    protected virtual void CommOnPacketReceived(IS2CPacket packet) { }
    

    public ReactiveCommand<Unit, IRoutableViewModel>? NavigateBackCommand { get; } =
        Router?.NavigateBack;
    

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
