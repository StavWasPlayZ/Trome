using System.Reactive.Threading.Tasks;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public abstract class SubRoomViewModel : PageViewModel
{
    public RoomModel RoomModel { get; }

    protected SubRoomViewModel(IScreen hostScreen, RoomModel roomModel) : base(hostScreen)
    {
        RoomModel = roomModel;
    }

    protected SubRoomViewModel()
    {
        RoomModel = RoomModel.CreateMockRoom(AppService.SessionUser!);
    }
    
    
    protected async Task NavBackFromRoom()
    {
        // Assuming Join -> Room -> Game
        await NavigateBackCommand!.Execute().ToTask();
        await NavigateBackCommand!.Execute().ToTask();
    }
    
    
    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case RoomClosedNotification:
                NavBackFromRoom().Wait();
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
}