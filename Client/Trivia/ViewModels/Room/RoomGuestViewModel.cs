using System;
using System.Collections.Generic;
using System.Reactive.Disposables;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;
using Trivia.Models.Raw;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Room;

public class RoomGuestViewModel : RoomViewModel
{
    public RoomGuestViewModel(IScreen hostScreen, RoomModel roomModel, List<User> players) :
        base(hostScreen, roomModel, players)
    {
        MaxPlayers = RoomModel.Data.MaxPlayers;
        
        this.WhenActivated(disposables =>
        {
            this
                .WhenAnyValue(x => x.RoomModel)
                .Subscribe(_ => MaxPlayers = RoomModel.Data.MaxPlayers)
                .DisposeWith(disposables);
        });
    }

    public RoomGuestViewModel()
    {}


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
            new GameCountdownViewModel(
                HostScreen,
                RoomModel with
                {
                    Data = gameStartedNotif.Data
                }
            )
        );
    }

    private void OnRoomDataUpdated(RoomDataUpdatedNotification roomDataNotif)
    {
        RoomModel = RoomModel with
        {
            Data = roomDataNotif.Data
        };
    }
}