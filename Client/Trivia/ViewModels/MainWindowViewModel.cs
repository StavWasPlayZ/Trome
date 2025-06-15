using System.Reactive;
using System.Reactive.Disposables;
using ReactiveUI;
using Trivia.ViewModels.Popups;

namespace Trivia.ViewModels;

public class MainWindowViewModel : ViewModelBase, IScreen
{
    public RoutingState Router { get; } = new();

    public ReactiveCommand<Unit, Unit> CloseDialogCommand { get; }
    
    
    public bool AllowClosing { get; set; }
    public bool IsCloseProcessing { get; set; }


    public MainWindowViewModel(bool isMock) : base(isMock)
    {
        this.WhenActivated((CompositeDisposable _) =>
        {
            Router.Navigate.Execute(
                !isMock
                    ? new ServerIpViewModel(this)
                    : new ServerIpViewModel()
            );
        });
        
        CloseDialogCommand = ReactiveCommand.Create(() =>
        {
            PopupContents = null;
        });
    }

    public MainWindowViewModel() : this(true)
    {
    }


    public void HandleClosing()
    {
        IsCloseProcessing = true;
        Router.NavigateAndReset.Execute(new ClosingViewModel(this));
    }


    private PopupViewModel? _popupContents;
    
    public PopupViewModel? PopupContents
    {
        get => _popupContents;
        set => this.RaiseAndSetIfChanged(ref _popupContents, value);
    }
}