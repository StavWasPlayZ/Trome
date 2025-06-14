using System.Reactive;
using ReactiveUI;

namespace Trivia.ViewModels.Popups;

public class PopupViewModel(bool isMock) : ViewModelBase(isMock)
{
    public ReactiveCommand<Unit, Unit> CloseCommand => MainWindowViewModel?.CloseDialogCommand ?? NoOpCommand;
}