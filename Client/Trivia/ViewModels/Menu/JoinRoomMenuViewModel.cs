using System.Collections.Generic;
using System.Reactive;
using System.Reactive.Disposables;
using System.Threading;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Menu;

public class JoinRoomMenuViewModel : PageViewModel
{
    private const int RefreshTime = 3000;
    
    
    public ReactiveCommand<Unit, Unit> NewRoomButtonCommand { get; }
    
    public ReactiveCommand<int, Unit> JoinRoomButtonCommand { get; }


    private List<Room> _rooms = [];

    public List<Room> Rooms
    {
        get => _rooms;
        private set => this.RaiseAndSetIfChanged(ref _rooms, value);
    }

    private Room? _room;
    
    public Room? SelectedRoom
    {
        get => _room;
        set => this.RaiseAndSetIfChanged(ref _room, value);
    }
    
    
    private bool _refreshRoomThreadRunning;

    public JoinRoomMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        NewRoomButtonCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            var response = await Comm.SendRequestAwaitResponse<CreateRoomResponse>(new CreateRoomRequest());
            
            NavigateTo(new CreateRoomViewModel(hostScreen, new Room
            {
                Id = response.RoomId,
                Admin = AppService.SessionUser!,
                PlayersCount = 1,
                Data = response.Data
            }));
        });
        
        JoinRoomButtonCommand = ReactiveCommand.CreateFromTask<int>(async roomId =>
        {
            //TODO: Handle room deleted before refresh
            var response = await Comm.SendRequestAwaitResponse<JoinRoomResponse>(new JoinRoomRequest(roomId));
            
            NavigateTo(new JoinedRoomViewModel(hostScreen, response.Room, response.Players));
        });

        this.WhenActivated(disposables =>
        {
            Disposable
                .Create(() => _refreshRoomThreadRunning = false)
                .DisposeWith(disposables);
        });
    }
    
    public JoinRoomMenuViewModel() : base(null!)
    {
        JoinRoomButtonCommand = ReactiveCommand.Create<int>(_ => { });
        NewRoomButtonCommand = NoOpCommand;
        Rooms = Room.GenerateMockRooms(30);
        SelectedRoom = Rooms[0];
    }


    public void RunRefreshRoomsThread()
    {
        if (_refreshRoomThreadRunning)
            return;
        
        _refreshRoomThreadRunning = true;
        new Thread(() => _ = RefreshRoomsThread()).Start();
    }
    

    private async Task RefreshRoomsThread()
    {
        while (_refreshRoomThreadRunning)
        {            
            var response = await Communicator.Instance.SendRequestAwaitResponse<GetRoomsResponse>(new GetRoomsRequest());

            Rooms = response.Rooms;
            Thread.Sleep(RefreshTime);
        }
    }
}
