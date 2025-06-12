using ReactiveUI;
using Trivia.Models;
using Trivia.Models.User;

namespace Trivia.ViewModels.Room;

public class HeadToHeadRoomViewModel : RoomViewModel
{
    public HeadToHeadUserModel Creator { get; }
    public HeadToHeadUserModel? Opponent { get; }
    
    public HeadToHeadRoomViewModel(IScreen hostScreen, RoomModel roomModel) :
        base(hostScreen, roomModel, [])
    {
        _roomName = roomModel.Data.Name;

        //TODO: Implement getting points
        var currUser = HeadToHeadUserModel.FromUser(AppService.SessionUser!, 0); 
        
        if (IsAdmin)
        {
            Creator = currUser;
            Opponent = null;
        }
        else
        {
            Creator = HeadToHeadUserModel.FromUser(roomModel.Admin, 0);
            Opponent = currUser;
        }
    }

    public HeadToHeadRoomViewModel()
    {
        _roomName = "ROOM NAME";
        
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
