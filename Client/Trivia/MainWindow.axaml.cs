using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Input;
using Avalonia.LogicalTree;
using Avalonia.ReactiveUI;
using ReactiveUI;
using Trivia.ViewModels;

namespace Trivia;

public partial class MainWindow : ReactiveWindow<MainWindowViewModel>
{
    public static IClassicDesktopStyleApplicationLifetime? ApplicationLifetime =>
        Application.Current?.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime;
    
    public static MainWindow? Instance => ApplicationLifetime?.MainWindow as MainWindow;
    
    
    public MainWindow()
    {
        this.WhenActivated(_ =>
        {
            ViewModel?.Router.Navigate.Execute(new ConnectingViewModel(ViewModel));
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

    private void OnDetachedFromLogicalTree(object? sender, LogicalTreeAttachmentEventArgs e)
    {
        ViewModel?.HandleClosing();
    }
}