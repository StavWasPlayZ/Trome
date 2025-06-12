using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;
using Trivia.Models.User;

namespace Trivia.ViewModels.Room;

public class HeadToHeadRoomViewModel : RoomViewModel
{
    public HeadToHeadRoomViewModel(IScreen hostScreen, RoomModel roomModel) :
        base(hostScreen, roomModel, [])
    {
        _roomName = roomModel.Data.Name;
        MaxPlayers = 2;

        UpdatePlayerFields();
    }

    public HeadToHeadRoomViewModel()
    {
        _roomName = "ROOM NAME";
        
        Creator = HeadToHeadUserModel.FromUser(AppService.SessionUser!, 0);
        Opponent = null;
    }


    private HeadToHeadUserModel _creator = null!;

    public HeadToHeadUserModel Creator
    {
        get => _creator;
        set => this.RaiseAndSetIfChanged(ref _creator, value);
    }

    private HeadToHeadUserModel? _opponent;

    public HeadToHeadUserModel? Opponent
    {
        get => _opponent;
        set => this.RaiseAndSetIfChanged(ref _opponent, value);
    }


    private void UpdatePlayerFields()
    {
        //TODO: Implement getting points
        Creator = HeadToHeadUserModel.FromUser(Players[0]!, 0);
        Opponent = (Players.Count > 1 && Players[1] is not null)
            ? HeadToHeadUserModel.FromUser(Players[1]!, 0)
            : null;
    }


    private string? _roomName;

    public string? RoomName
    {
        get => _roomName;
        set => this.RaiseAndSetIfChanged(ref _roomName, value);
    }


    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        base.CommOnPacketReceived(packet);

        if (packet is PlayerJoinedRoomNotification or PlayerLeftRoomNotification)
        {
            UpdatePlayerFields();
        }
    }
}
