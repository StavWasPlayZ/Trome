using ReactiveUI;
using Trivia.ViewModels.Game;

namespace Trivia.Views.Game;

public partial class CreateRoomView : PageViewControl<CreateRoomViewModel>
{
    public CreateRoomView()
    {
        InitializeComponent();
        
        this.WhenActivated(_ => { });
    }
}