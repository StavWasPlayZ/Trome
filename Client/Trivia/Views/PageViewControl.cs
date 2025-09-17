using Avalonia.ReactiveUI;
using Trivia.ViewModels;

namespace Trivia.Views;

public abstract class PageViewControl<TPvm> : ReactiveUserControl<TPvm> where TPvm : PageViewModel
{
    protected static MainWindowViewModel? MainWindowViewModel => Utils.MainWindowViewModelInstance;
}