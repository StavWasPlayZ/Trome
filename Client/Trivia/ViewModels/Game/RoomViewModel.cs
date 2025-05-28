using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models.Raw;
using Trivia.Models.User;

namespace Trivia.ViewModels.Game;

public abstract class RoomViewModel : PageViewModel
{
    public ObservableCollection<RoomUserModel?> Players { get; }
    
    private Room _room;

    public Room Room
    {
        get => _room;
        set => this.RaiseAndSetIfChanged(ref _room, value);
    }


    /// <summary>
    /// </summary>
    /// <param name="hostScreen"></param>
    /// <param name="room"></param>
    /// <param name="players">
    /// A list of all already existing players.
    /// Must be present if the room's <see cref="Room.PlayersCount"/> is greater than 1.
    /// </param>
    protected RoomViewModel(IScreen hostScreen, Room room, List<User>? players = null) : base(hostScreen)
    {
        _room = room;

        var isRoomAdmin = room.Admin == AppService.SessionUser!;
        
        // This assumes that the first player is always the admin.
        // Also note that ObservableCollection does not provide an API for setting a capacity.
        Players = [
            RoomUserModel.FromUser(room.Admin, isRoomAdmin, true)
        ];

        // The current player is not to be provided.
        for (var i = 1; i < room.PlayersCount - 1; i++)
        {
            var player = players![i];
            Players.Add(RoomUserModel.FromUser(player, player == AppService.SessionUser!, false));
        }
        
        // If we were not added during the initial addition as an Admin, now's the time.
        // (AKA we're last in the list)
        if (!isRoomAdmin)
        {
            Players.Add(RoomUserModel.FromUser(AppService.SessionUser!, true, false));
        }

        for (var i = room.PlayersCount; i < room.Data.MaxPlayers; i++)
        {
            Players.Add(null);
        }
    }

    protected RoomViewModel()
    {
        _room = Room.CreateMockRoom(AppService.SessionUser!);
        
        Players = new ObservableCollection<RoomUserModel?>(
            Enumerable.Range(1, 10)
                .Select(i =>
                {
                    if (i > 5)
                        return null;
                    
                    return new RoomUserModel
                    {
                        Id = i,
                        IsAdmin = i == 1,
                        IsCurrentUser = i == 1,
                        Username = $"User {i}"
                    };
                })
        );
    }
    

    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case PlayerJoinedRoomNotification playerJoinedRoomNotif:
                HandlePlayerJoined(playerJoinedRoomNotif);
                break;
            
            case PlayerLeftRoomNotification playerLeftRoomNotif:
                HandlePlayerLeft(playerLeftRoomNotif);
                break;
        }
    }

    private void HandlePlayerJoined(PlayerJoinedRoomNotification playerJoinedRoomNotif)
    {
        Players[Room.PlayersCount] = RoomUserModel.FromUser(playerJoinedRoomNotif.Player, false, false);
        
        Room = Room with
        {
            PlayersCount = _room.PlayersCount + 1
        };
    }

    private void HandlePlayerLeft(PlayerLeftRoomNotification playerLeftRoomNotif)
    {
        for (var i = 0; i < Players.Count; i++)
        {
            if (Players[i]!.Id != playerLeftRoomNotif.PlayerId)
                continue;
            
            Players.RemoveAt(i);
            break;
        }
        
        Players.Add(null);
        
        Room = Room with
        {
            PlayersCount = _room.PlayersCount - 1
        };
    }
}
