using System;
using System.Collections.Generic;
using System.Reactive;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models;
using Trivia.Models.Raw;
using Trivia.Models.User;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Room;

public class HeadToHeadRoomViewModel : RoomViewModel
{
    public ReactiveCommand<Unit, Unit> StartGameCommand { get; }

    public HeadToHeadRoomViewModel(IScreen hostScreen, RoomModel roomModel, List<User> players) :
        base(hostScreen, roomModel, players)
    {
        _roomName = roomModel.Data.Name;
        MaxPlayers = 2;
        
        StartGameCommand = ReactiveCommand.CreateFromTask(
            StartGame,
            this.WhenAnyValue(x => x.Opponent)
                .Select(opponent => opponent is not null)
        );

        UpdatePlayerFields();
        
        
        this.WhenActivated(disposables =>
        {
            this
                .WhenAnyValue(x => x.RoomName)
                .Skip(1)
                .DistinctUntilChanged()
                .Throttle(RoomDataUpdateDelay)
                .Subscribe(newName => UpdateAndSendRoomData(RoomModel.Data with
                {
                    Name = newName ?? "",
                }))
                .DisposeWith(disposables);
        });
    }

    public HeadToHeadRoomViewModel()
    {
        StartGameCommand = NoOpCommand;
        
        _roomName = "ROOM NAME";
        
        Creator = HeadToHeadUserModel.FromUser(AppService.SessionUser!, 0);
        Opponent = null;
    }
    
    
    private async Task StartGame()
    {
        await Comm.SendRequestAsync<StartGameResponse>(new StartGameRequest(RoomModel.Data));

        NavigateTo(new GameCountdownViewModel(HostScreen, RoomModel));
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
        switch (packet)
        {
            case GameStartedNotification gameStartedNotif:
                NavigateTo(new GameCountdownViewModel(HostScreen, RoomModel with
                {
                    Data = gameStartedNotif.Data
                }));
                break;
            
            case RoomDataUpdatedNotification roomDataUpdatedNotif:
                RoomModel = RoomModel with
                {
                    Data = roomDataUpdatedNotif.Data
                };
                RoomName = RoomModel.Data.Name;
                break;
            
            case PlayerJoinedRoomNotification:
            case PlayerLeftRoomNotification:
                UpdatePlayerFields();
                break;
        }
        
        base.CommOnPacketReceived(packet);
    }
}
