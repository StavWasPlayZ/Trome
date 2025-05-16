using Avalonia.Controls;
using ReactiveUI;

namespace Trivia.ViewModels;

public class MainWindowViewModel : ViewModelBase, IScreen
{
    public RoutingState Router { get; } = new();

    
    private Control? _popupContents;
    
    public Control? PopupContents
    {
        get => _popupContents;
        set => this.RaiseAndSetIfChanged(ref _popupContents, value);
    }
}