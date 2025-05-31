using ReactiveUI;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class FinishedEarlyViewModel : GameViewModelBase
{
    private static readonly string[] EndingLines = [
        "You await the inferiors before you...",
        "You thought they gave you all they had, yet they knew naught...",
        "\"Is that all?,\" you think to yourself...",
        "“I held back,” you think to yourself...",
        "You suppressed your power, lest the world trembled once more...",
        "They call it a game, but to you, it's merely breathing...",
        "They trained for years, but they never trained for you...",
        "While they reached for greatness, you had already forgotten it...",
        "Their struggle was just your warm-up...",
        "You thought you faced their final form. How quaint...",
    ];

    public string EndingLine { get; } = EndingLines[App.Random.Next(EndingLines.Length)];

    public Room Room { get; }
    
    public FinishedEarlyViewModel(IScreen hostScreen, Room room, int playersFinished) : base(hostScreen)
    {
        Room = room;
        _playersFinished = playersFinished;
    }

    public FinishedEarlyViewModel()
    {
        Room = Room.CreateMockRoom(AppService.SessionUser!);
        _playersFinished = 2;
    }


    protected override void CommOnPacketReceived(IS2CPacket packet)
    {
        switch (packet)
        {
            case GameEndedNotification:
                NavigateTo(new AfterGameViewModel(HostScreen));
                break;
            
            case PlayerFinishedNotification:
                _playersFinished++;
                break;
            
            default:
                base.CommOnPacketReceived(packet);
                break;
        }
    }
    
    private int _playersFinished;

    public int PlayersFinished
    {
        get => _playersFinished;
        set => this.RaiseAndSetIfChanged(ref _playersFinished, value);
    }
}