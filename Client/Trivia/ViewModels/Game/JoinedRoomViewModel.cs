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

        _data = Room.Data;
    }

    public JoinedRoomViewModel()
    {
        LeaveRoomCommand = NoOpCommand;
        _data = Room.Data;
    }


    private RoomData _data;

    public RoomData Data
    {
        get => _data;
        set => this.RaiseAndSetIfChanged(ref _data, value);
    }


    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case RoomClosedNotification:
                NavigateBackCommand!.Execute();
                break;
            
            case RoomDataUpdatedNotification roomDataNotif:
                Data = Room.Data = roomDataNotif.Data;
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
}