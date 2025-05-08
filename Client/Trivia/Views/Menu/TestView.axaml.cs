using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class TestView : PageViewControl<TestViewModel>
{
    public TestView()
    {
        this.WhenActivated(_ => { });
        
        InitializeComponent();
    }
}