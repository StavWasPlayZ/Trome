using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models;
using Trivia.Models.User;

namespace Trivia.ViewModels.Room;

public class HeadToHeadRoomViewModel : RoomViewModel
{   
    public ReactiveCommand<Unit, Unit> LeaveRoomCommand { get; }
    
    public bool IsCreator { get; }


    public HeadToHeadUserModel Creator { get; }
    public HeadToHeadUserModel? Opponent { get; }
    
    /// <param name="hostScreen"></param>
    /// <param name="roomModel"></param>
    /// <param name="creator">If an external creator exists, they may be passed here.</param>
    public HeadToHeadRoomViewModel(IScreen hostScreen, RoomModel roomModel, HeadToHeadUserModel? creator) :
        base(hostScreen, roomModel, [])
    {
        LeaveRoomCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<LeaveRoomResponse>(new LeaveRoomRequest());
            
            NavigateBackCommand!.Execute();
        });

        // If we were not supplied an external creator, then we are the creator.
        IsCreator = creator is null;
        var currUser = HeadToHeadUserModel.FromUser(AppService.SessionUser!, 0); 
        
        //TODO: Implement getting points
        if (IsCreator)
        {
            Creator = currUser;
            Opponent = null;
        }
        else
        {
            Creator = creator!;
            Opponent = currUser;
        }
    }

    public HeadToHeadRoomViewModel()
    {
        LeaveRoomCommand = NoOpCommand;

        _roomName = "ROOM NAME";
        
        IsCreator = true;
        Creator = HeadToHeadUserModel.FromUser(AppService.SessionUser!, 0);
        Opponent = null;
    }


    private string? _roomName;

    public string? RoomName
    {
        get => _roomName;
        set => this.RaiseAndSetIfChanged(ref _roomName, value);
    }
}