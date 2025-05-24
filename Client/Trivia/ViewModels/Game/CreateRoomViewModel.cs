using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class CreateRoomViewModel : RoomViewModel
{
    public ReactiveCommand<Unit, Unit> CloseRoomCommand { get; }

    public CreateRoomViewModel(IScreen hostScreen, Room room) : base(hostScreen, room)
    {
        CloseRoomCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            await Comm.SendRequestAwaitResponse<CloseRoomResponse>(new CloseRoomRequest());
            
            NavigateBackCommand!.Execute();
        });
    }

    public CreateRoomViewModel()
    {
        CloseRoomCommand = NoOpCommand;
    }
}