using Avalonia;
using Avalonia.Input;
using Avalonia.ReactiveUI;
using ReactiveUI;
using Trivia.ViewModels;
using Trivia.ViewModels.Auth;

namespace Trivia;

public partial class MainWindow : ReactiveWindow<MainWindowViewModel>
{
    public MainWindow()
    {
        this.WhenActivated(_ =>
        {
            ViewModel?.Router.Navigate.Execute(new LoginViewModel(ViewModel));
        });
        
        InitializeComponent();
    }

    private void PopupControl_OnPropertyChanged(object? sender, AvaloniaPropertyChangedEventArgs e)
    {
        if (e.Property != ContentProperty)
            return;

        ScreenDarkener.IsVisible = e.NewValue != null;
    }

    private void ScreenDarkener_OnPointerReleased(object? sender, PointerReleasedEventArgs pointerReleasedEventArgs)
    {
        ViewModel!.PopupContents = null;
    }
}