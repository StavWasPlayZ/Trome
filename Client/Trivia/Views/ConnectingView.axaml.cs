using Avalonia.ReactiveUI;
using ReactiveUI;
using Trivia.ViewModels;

namespace Trivia.Views;

public partial class ConnectingView : ReactiveUserControl<ConnectingViewModel>
{
    public ConnectingView()
    {
        InitializeComponent();

        this.WhenActivated(_ => { });
    }
}