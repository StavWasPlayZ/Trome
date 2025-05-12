using ReactiveUI;
using Trivia.ViewModels.Menu;

namespace Trivia.Views.Menu;

public partial class CreateRoomView : PageViewControl<CreateRoomViewModel>
{
    public CreateRoomView()
    {
        this.WhenActivated(_ => { });
        
        InitializeComponent();
    }
}