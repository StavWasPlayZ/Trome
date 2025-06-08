using System.Reactive;
using System.Reactive.Disposables;
using Avalonia.Controls;
using ReactiveUI;

namespace Trivia.ViewModels;

public class MainWindowViewModel : ViewModelBase, IScreen, IActivatableViewModel
{
    public ViewModelActivator Activator { get; } = new();

    public RoutingState Router { get; } = new();

    public ReactiveCommand<Unit, Unit>? CloseDialogCommand { get; }


    public MainWindowViewModel()
    {
        this.WhenActivated((CompositeDisposable _) =>
        {
            Router.Navigate.Execute(
                // If we're in design mode, strictly use the mock constructor instead.
                !Design.IsDesignMode
                    ? new ConnectingViewModel(this)
                    : new ConnectingViewModel()
            );
        });
        
        CloseDialogCommand = ReactiveCommand.Create(() =>
        {
            PopupContents = null;
        });
    }


    public void HandleClosing()
    {
        Router.NavigateAndReset.Execute(new ClosingViewModel(this));
        
        App.MusicService.Dispose();
        Communicator.Instance.Dispose();
    }


    private Control? _popupContents;
    
    public Control? PopupContents
    {
        get => _popupContents;
        set => this.RaiseAndSetIfChanged(ref _popupContents, value);
    }
}