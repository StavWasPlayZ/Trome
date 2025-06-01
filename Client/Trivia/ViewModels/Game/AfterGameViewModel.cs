using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class AfterGameViewModel : SubRoomViewModel
{
    private RoomModel _room;

    public AfterGameViewModel(IScreen hostScreen, RoomModel room) : base(hostScreen)
    {
        _room = room;
    }

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
                NavigateTo(new GameViewModel(HostScreen, _room));
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
}