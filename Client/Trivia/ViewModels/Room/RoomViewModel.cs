using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reactive.Disposables;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models.Raw;
using Trivia.Models.User;

namespace Trivia.ViewModels.Room;

public abstract class RoomViewModel : PageViewModel
{
    public ObservableCollection<RoomUserModel?> Players { get; }
    
    private RoomModel _roomModel;

    public RoomModel RoomModel
    {
        get => _roomModel;
        set => this.RaiseAndSetIfChanged(ref _roomModel, value);
    }


    /// <summary>
    /// </summary>
    /// <param name="hostScreen"></param>
    /// <param name="roomModel"></param>
    /// <param name="players">
    /// A list of all already existing players.
    /// Must be present if the room's <see cref="RoomModel.PlayersCount"/> is greater than 1.
    /// </param>
    protected RoomViewModel(IScreen hostScreen, RoomModel roomModel, List<User>? players = null) : base(hostScreen)
    {
        _roomModel = roomModel;

        var isRoomAdmin = roomModel.Admin == AppService.SessionUser!;
        
        // This assumes that the first player is always the admin.
        // Also note that ObservableCollection does not provide an API for setting a capacity.
        Players = [
            RoomUserModel.FromUser(roomModel.Admin, isRoomAdmin, true)
        ];

        // The current player is not to be provided.
        for (var i = 1; i < roomModel.PlayersCount - 1; i++)
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

        for (var i = roomModel.PlayersCount; i < roomModel.Data.MaxPlayers; i++)
        {
            Players.Add(null);
        }
        
        
        this.WhenActivated(disposables =>
        {
            this
                .WhenAnyValue(x => x.MaxPlayers)
                .Subscribe(_ => UpdatePlayersList())
                .DisposeWith(disposables);
        });
    }

    protected RoomViewModel()
    {
        _roomModel = RoomModel.CreateMockRoom(AppService.SessionUser!);
        
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
    
    
    private int _maxPlayers;

    public int MaxPlayers
    {
        get => _maxPlayers;
        set => this.RaiseAndSetIfChanged(ref _maxPlayers, value);
    }
    
    
    private void UpdatePlayersList()
    {
        if (MaxPlayers == Players.Count)
            return;

        while (MaxPlayers > Players.Count)
        {
            Players.Add(null);
        }
        
        while (MaxPlayers < Players.Count)
        {
            var current = Players.Last();
            
            // If we're about to destroy a player, just don't.
            if (current is not null)
                break;

            Players.Remove(current);
        }
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
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }

    private void HandlePlayerJoined(PlayerJoinedRoomNotification playerJoinedRoomNotif)
    {
        Players[RoomModel.PlayersCount] = RoomUserModel.FromUser(playerJoinedRoomNotif.Player, false, false);
        
        RoomModel = RoomModel with
        {
            PlayersCount = _roomModel.PlayersCount + 1
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
        
        // Return the missing player slot (if necessary)
        if (Players.Count == MaxPlayers - 1)
        {
            Players.Add(null);
        }
        
        RoomModel = RoomModel with
        {
            PlayersCount = _roomModel.PlayersCount - 1
        };
    }
}
