using System;
using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Game;

public class FinishedEarlyViewModel : GameViewModelBase
{
    private static readonly string[] EndingLines = [
        "You await the inferiors before you...",
        "You thought they gave you all they had, yet they knew naught...",
        "\"Is that all?,\" you think to yourself...",
        "\"I held back,\" you think to yourself...",
        "You suppress your power, lest the world trembled once more...",
        "They call it a game, but to you, it's merely breathing...",
        "They trained for years, but they never trained for you...",
        "While they reached for greatness, you had already forgotten it...",
        "Their struggle was just your warm-up...",
        "You thought you faced their final form. How quaint...",
    ];

    public string EndingLine { get; } = EndingLines[App.Random.Next(EndingLines.Length)];
    
    public FinishedEarlyViewModel(IScreen hostScreen, RoomModel roomModel, int playersFinished) :
        base(hostScreen, roomModel)
    {
        PlayersFinished = playersFinished;
    }

    public FinishedEarlyViewModel()
    {}


    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case GameEndedNotification gameEndedNotif:
                NavigateAndPop(new AfterGameViewModel(HostScreen, RoomModel, gameEndedNotif.Results))!.Subscribe();
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
}