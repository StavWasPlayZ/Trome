using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
    private PageViewModel _currentPage = new MainMenuViewModel();

    public PageViewModel CurrentPage
    {
        get => _currentPage;
        set => this.RaiseAndSetIfChanged(ref _currentPage, value);
    }
}