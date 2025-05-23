using ReactiveUI;
using Trivia.ViewModels.Game;

namespace Trivia.Views.Game;

public partial class CreateRoomView : RoomViewControl<CreateRoomViewModel>
{
    public CreateRoomView()
    {
        InitializeComponent();
        
        this.WhenActivated(_ => { });
    }
}