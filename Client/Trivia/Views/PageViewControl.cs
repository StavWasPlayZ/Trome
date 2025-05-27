using System.Reactive.Disposables;
using Avalonia.LogicalTree;
using Avalonia.ReactiveUI;
using ReactiveUI;
using Trivia.ViewModels;

namespace Trivia.Views;

public class PageViewControl<TPvm> : ReactiveUserControl<TPvm> where TPvm : PageViewModel
{
    protected PageViewControl()
    {
        AttachedToLogicalTree += OnAttachedToLogicalTree;

        this.WhenActivated(disposables =>
        {
            Disposable
                .Create(() => AttachedToLogicalTree -= OnAttachedToLogicalTree)
                .DisposeWith(disposables);
        });
    }
    
    private void OnAttachedToLogicalTree(object? sender, LogicalTreeAttachmentEventArgs e)
    {
        ViewModel?.SubToServerEvents();
    }
}