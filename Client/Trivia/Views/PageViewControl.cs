using Avalonia.ReactiveUI;
using Trivia.ViewModels;

namespace Trivia.Views;

public class PageViewControl<TPvm> : ReactiveUserControl<TPvm> where TPvm : PageViewModel
{
    
}