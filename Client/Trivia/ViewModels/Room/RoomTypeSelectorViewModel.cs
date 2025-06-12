using System;
using System.Reactive;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models;

namespace Trivia.ViewModels.Room;

public class RoomTypeSelectorViewModel : PageViewModel
{
    public ReactiveCommand<RoomType, Unit> CreateRoomCommand { get; }
    
    public RoomTypeSelectorViewModel(IScreen hostScreen) : base(hostScreen)
    {
        CreateRoomCommand = ReactiveCommand.CreateFromTask<RoomType>(async (roomType, _) =>
        {
            await CreateRoom(roomType);
        });
    }

    public RoomTypeSelectorViewModel()
    {
        CreateRoomCommand = ReactiveCommand.Create<RoomType>(_ => { });
    }


    private async Task CreateRoom(RoomType roomType)
    {
        var response = await Comm.SendRequestAwaitResponse<CreateRoomResponse>(new CreateRoomRequest(roomType));

        var roomModel = new RoomModel
        {
            Id = response.RoomId,
            Admin = AppService.SessionUser!,
            PlayersCount = 1,
            Data = response.Data
        };
        
        NavigateTo(roomType switch
        {
            RoomType.TriviaRush => new RoomAdminViewModel(HostScreen, roomModel),
            RoomType.HeadToHead => new HeadToHeadRoomViewModel(HostScreen, roomModel, true),
            
            _ => throw new ArgumentOutOfRangeException(nameof(roomType), roomType, null)
        });
    }
}