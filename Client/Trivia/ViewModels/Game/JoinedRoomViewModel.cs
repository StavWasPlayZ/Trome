using System;
using System.Collections.Generic;
using System.Reactive;
using System.Reactive.Disposables;
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
        
        this.WhenActivated(disposables =>
        {
            this
                .WhenAnyValue(x => x.Room)
                .Subscribe(_ => MaxPlayers = Room.Data.MaxPlayers)
                .DisposeWith(disposables);
        });
    }

    public JoinedRoomViewModel()
    {
        LeaveRoomCommand = NoOpCommand;
    }


    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case RoomClosedNotification:
                NavigateBackCommand!.Execute();
                break;
            
            case RoomDataUpdatedNotification roomDataNotif:
                OnRoomDataUpdated(roomDataNotif);
                break;
            
            case GameStartedNotification gameStartedNotif:
                OnGameStarted(gameStartedNotif);
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }

    private void OnGameStarted(GameStartedNotification gameStartedNotif)
    {
        NavigateTo(
            new GameViewModel(
                HostScreen,
                Room with
                {
                    Data = gameStartedNotif.Data
                }
            )
        );
    }

    private void OnRoomDataUpdated(RoomDataUpdatedNotification roomDataNotif)
    {
        Room = Room with
        {
            Data = roomDataNotif.Data
        };
    }
}