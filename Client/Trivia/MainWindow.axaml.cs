using Avalonia.ReactiveUI;
using ReactiveUI;
using Trivia.ViewModels;
using Trivia.ViewModels.Menu;

namespace Trivia;

public partial class MainWindow : ReactiveWindow<MainWindowViewModel>
{
    public MainWindow()
    {
        this.WhenActivated(_ =>
        {
            var vm = DataContext as MainWindowViewModel;
            vm?.Router.Navigate.Execute(new TestViewModel(vm));
        });
        
        InitializeComponent();
    }
}