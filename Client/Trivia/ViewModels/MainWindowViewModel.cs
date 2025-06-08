using System.Reactive;
using Avalonia.Controls;
using ReactiveUI;

namespace Trivia.ViewModels;

public class MainWindowViewModel : ViewModelBase, IScreen
{
    public RoutingState Router { get; } = new();

    public ReactiveCommand<Unit, Unit>? CloseDialogCommand { get; }


    public MainWindowViewModel()
    {
        CloseDialogCommand = ReactiveCommand.Create(() =>
        {
            PopupContents = null;
        });
    }


    public void HandleClosing()
    {
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