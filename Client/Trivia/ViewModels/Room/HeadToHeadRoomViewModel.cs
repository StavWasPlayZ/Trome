using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Room;

public class HeadToHeadRoomViewModel : RoomViewModel
{   
    public ReactiveCommand<Unit, Unit> LeaveRoomCommand { get; }
    
    public bool IsCreator { get; }

    public HeadToHeadRoomViewModel(IScreen hostScreen, RoomModel roomModel, bool isCreator) :
        base(hostScreen, roomModel, [])
    {
        LeaveRoomCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<LeaveRoomResponse>(new LeaveRoomRequest());
            
            NavigateBackCommand!.Execute();
        });
        
        IsCreator = isCreator;
    }

    public HeadToHeadRoomViewModel()
    {
        LeaveRoomCommand = NoOpCommand;
        
        IsCreator = true;
    }
}