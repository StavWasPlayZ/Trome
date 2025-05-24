using System.Collections.Generic;
using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class JoinedRoomViewModel : RoomViewModel
{
    public ReactiveCommand<Unit, Unit> LeaveRoomCommand { get; }
    
    public JoinedRoomViewModel(IScreen hostScreen, Room room, List<User> players) : base(hostScreen, room, players)
    {
        LeaveRoomCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<LeaveRoomResponse>(new LeaveRoomRequest());
            
            NavigateBackCommand!.Execute();
        });
    }

    public JoinedRoomViewModel()
    {
        LeaveRoomCommand = NoOpCommand;
    }


    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        if (packet is RoomClosedNotification)
        {
            NavigateBackCommand!.Execute();
            return;
        }
        
        base.CommOnPacketReceived(packet);
    }
}