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
    
    
    public bool AllowClosing { get; set; }
    public bool IsCloseProcessing { get; set; }


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
        IsCloseProcessing = true;
        Router.NavigateAndReset.Execute(new ClosingViewModel(this));
    }


    private Control? _popupContents;
    
    public Control? PopupContents
    {
        get => _popupContents;
        set => this.RaiseAndSetIfChanged(ref _popupContents, value);
    }
}