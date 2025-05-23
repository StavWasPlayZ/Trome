using ReactiveUI;
using Trivia.ViewModels.Game;

namespace Trivia.Views.Game;

public partial class JoinedRoomView : RoomViewControl<JoinedRoomViewModel>
{
    public JoinedRoomView()
    {
        InitializeComponent();
        
        this.WhenActivated(_ => { });
    }
}