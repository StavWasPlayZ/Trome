using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reactive;
using System.Reactive.Disposables;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models;
using Trivia.Models.Raw;
using Trivia.Models.User;

namespace Trivia.ViewModels.Room;

public abstract class RoomViewModel : PageViewModel
{
    protected static readonly TimeSpan RoomDataUpdateDelay = TimeSpan.FromMilliseconds(300);
    
    private bool _wasInitiated;
    
    public bool IsAdmin { get; }
    
    public ReactiveCommand<Unit, Unit> LeaveRoomCommand { get; }

    /// <summary>
    /// </summary>
    /// <param name="hostScreen"></param>
    /// <param name="roomModel"></param>
    /// <param name="players">
    /// A list of all already existing players.
    /// Excludes the current player.
    /// </param>
    protected RoomViewModel(IScreen hostScreen, RoomModel roomModel, List<User> players) :
        base(hostScreen)
    {
        IsAdmin = AppService.SessionUser! == roomModel.Admin;
        
        LeaveRoomCommand = ReactiveCommand.CreateFromTask(LeaveRoom);
        
        
        _roomModel = roomModel;
        _players = [];
        
        // The player list excludes the current player.
        // Add them back.
        players =
        [
            ..players,
            AppService.SessionUser!
        ];

        ReAddAllPlayers(players);        
        
        
        this.WhenActivated(disposables =>
        {
            this
                .WhenAnyValue(x => x.MaxPlayers)
                .Subscribe(_ => UpdateEmptyPlayerSlots())
                .DisposeWith(disposables);
            
            // Even though we've already got the existing players, still get them here.
            // This is because the provided list could very well be outdated, due to
            // navigation cache.

            if (_wasInitiated)
            {
                FetchPlayersInRoom()
                    .DisposeWith(disposables);
            }

            _wasInitiated = true;
        });
    }

    protected RoomViewModel()
    {
        _roomModel = RoomModel.CreateMockRoom(AppService.SessionUser!);

        LeaveRoomCommand = NoOpCommand;
        
        _players = new ObservableCollection<RoomUserModel?>(
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

        IsAdmin = true;
    }
    
    
    private async Task LeaveRoom()
    {
        if (IsAdmin)
        {
            await Comm.SendRequestAsync<CloseRoomResponse>(new CloseRoomRequest());
        }
        else
        {
            await Comm.SendRequestAsync<LeaveRoomResponse>(new LeaveRoomRequest());
        }
        
        ReturnToRooms();
    }

    private void ReturnToRooms()
    {
        NavigateBackCommand!.Execute().Subscribe();
    }
    
    
    protected void UpdateAndSendRoomData(RoomData newData)
    {
        if (!IsAdmin)
            return;
        
        RoomModel = RoomModel with
        {
            Data = newData
        }; 
        
        Comm.SendRequest(new UpdateRoomDataRequest(RoomModel.Data));
    } 


    private void ReAddAllPlayers(List<User> players)
    {
        List<RoomUserModel?> newPlayersList = [];

        newPlayersList.AddRange(
            players
                .Select(player =>
                    RoomUserModel.FromUser(
                        player,
                        player == AppService.SessionUser!,
                        player == RoomModel.Admin
                    )
                )
            );

        SetPlayersList(newPlayersList);
    }


    private async Task FetchPlayersInRoom()
    {
        var response = await Comm.SendRequestAsync<GetPlayersInRoomResponse>(new GetPlayersInRoomRequest());
        ReAddAllPlayers([..response.Players]);

        RoomModel = RoomModel with
        {
            PlayersCount = response.Players.Count
        };
    }
    
    
    private int _maxPlayers;

    public int MaxPlayers
    {
        get => _maxPlayers;
        set => this.RaiseAndSetIfChanged(ref _maxPlayers, value);
    }
    
    private RoomModel _roomModel;

    public RoomModel RoomModel
    {
        get => _roomModel;
        set => this.RaiseAndSetIfChanged(ref _roomModel, value);
    }
    
    private ObservableCollection<RoomUserModel?> _players;
    
    public ObservableCollection<RoomUserModel?> Players
    {
        get => _players;
        set => this.RaiseAndSetIfChanged(ref _players, value);
    }
    
    
    /// <summary>
    /// Adapts the provided players list to be displayable in the Room view,
    /// and sends an update notification to it.
    /// </summary>
    /// <param name="players">A raw list of players</param>
    private void SetPlayersList(List<RoomUserModel?> players)
    {
        UpdateEmptyPlayerSlotsFor(players);
        Players = new ObservableCollection<RoomUserModel?>(players);
    }

    private void UpdateEmptyPlayerSlotsFor(List<RoomUserModel?> players)
    {
        if (MaxPlayers == players.Count)
            return;

        while (MaxPlayers > players.Count)
        {
            players.Add(null);
        }
        
        while (MaxPlayers < players.Count)
        {
            var current = players.Last();
            
            // If we're about to destroy a player, just don't.
            if (current is not null)
                break;

            players.Remove(current);
        }
    }

    private void UpdateEmptyPlayerSlots()
    {
        SetPlayersList([..Players]);
    }
    

    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case RoomClosedNotification:
                NavigateBackCommand!.Execute();
                break;
            
            case PlayerJoinedRoomNotification playerJoinedRoomNotif:
                HandlePlayerJoined(playerJoinedRoomNotif);
                break;
            
            case PlayerLeftRoomNotification playerLeftRoomNotif:
                HandlePlayerLeft(playerLeftRoomNotif.PlayerId);
                break;
            
            case PlayerKickedNotification playerKickedNotif:
                HandlePlayerKicked(playerKickedNotif);
                break;
        }
        
        base.CommOnPacketReceived(packet);
    }

    private void HandlePlayerKicked(PlayerKickedNotification playerKickedNotif)
    {
        if (playerKickedNotif.PlayerId == AppService.SessionUser!.Id)
        {
            // This user was kicked
            ReturnToRooms();
            return;
        }
        
        HandlePlayerLeft(playerKickedNotif.PlayerId);
    }

    private void HandlePlayerJoined(PlayerJoinedRoomNotification playerJoinedRoomNotif)
    {
        Players[RoomModel.PlayersCount] = RoomUserModel.FromUser(playerJoinedRoomNotif.Player, false, false);
        
        RoomModel = RoomModel with
        {
            PlayersCount = _roomModel.PlayersCount + 1
        };
    }
    
    private void HandlePlayerLeft(int userId)
    {
        for (var i = 0; i < Players.Count; i++)
        {
            if (Players[i]!.Id != userId)
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
