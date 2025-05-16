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
            var vm = DataContext as MainWindowViewModel;
            vm?.Router.Navigate.Execute(new LoginViewModel(vm));
        });
        
        InitializeComponent();
    }
}