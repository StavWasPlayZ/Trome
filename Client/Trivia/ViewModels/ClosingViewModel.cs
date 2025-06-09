using System.Reactive.Disposables;
using System.Threading.Tasks;
using Avalonia.Threading;
using ReactiveUI;

namespace Trivia.ViewModels;

public class ClosingViewModel : PageViewModel
{
    public ClosingViewModel(IScreen hostScreen) : base(hostScreen)
    {
        this.WhenActivated((CompositeDisposable disposables) =>
        {
            _ = PerformCloseAsync();
        });
    }

    public ClosingViewModel() { }

    
    private static async Task PerformCloseAsync()
    {
        await Task.Run(DisposeStuff);

        await Dispatcher.UIThread.InvokeAsync(() =>
        {
            MainWindowViewModel!.AllowClosing = true;
            MainWindow.Instance!.Close();
        });
    }

    private static void DisposeStuff()
    {
        App.MusicService?.Dispose();
        Communicator.Instance.Dispose();
    }
}