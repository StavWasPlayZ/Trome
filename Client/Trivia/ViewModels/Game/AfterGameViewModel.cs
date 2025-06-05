using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public class AfterGameViewModel : SubRoomViewModel
{
    public AfterGameViewModel(IScreen hostScreen, RoomModel roomModel) : base(hostScreen, roomModel)
    {}

    public AfterGameViewModel()
    {}
    
    
    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case RoomClosedNotification:
                NavBackFromRoom().Wait();
                break;
            
            case GameStartedNotification:
                // tried to do: NavigateAndPop(new GameViewModel(HostScreen, _room))!.Subscribe(); 
                // got error: Method 'Subscribe' has 1 parameter(s) but is invoked with 0 argument(s)
                NavigateTo(new GameViewModel(HostScreen, RoomModel));
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
}