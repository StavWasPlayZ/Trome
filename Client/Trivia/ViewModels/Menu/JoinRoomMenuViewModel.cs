using System.Collections.Generic;
using System.Reactive;
using System.Reactive.Disposables;
using System.Threading;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C.Response;
using Trivia.Models.Raw;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Menu;

public class JoinRoomMenuViewModel : PageViewModel, IActivatableViewModel
{
    private const int RefreshTime = 3000;
    
    public ViewModelActivator Activator { get; } = new();
    
    
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
    
    
    private bool _isDisposed;

    public JoinRoomMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        NewRoomButtonCommand = ReactiveCommand.CreateFromTask(async () =>
        {
            var response = await Comm.SendRequestAwaitResponse<CreateRoomResponse>(new CreateRoomRequest());
            
            NavigateTo(new CreateRoomViewModel(hostScreen, new Room
            {
                Id = response.RoomId,
                Admin = AppService.SessionUser!,
                Data = response.Data
            }));
        });
        
        JoinRoomButtonCommand = ReactiveCommand.CreateFromTask<int>(async roomId =>
        {
            //TODO: Handle room deleted before refresh
            var response = await Comm.SendRequestAwaitResponse<JoinRoomResponse>(new JoinRoomRequest(roomId));
            
            NavigateTo(new JoinedRoomViewModel(hostScreen, response.Room));
        });

        this.WhenActivated(disposables =>
        {
            Disposable
                .Create(() => _isDisposed = true)
                .DisposeWith(disposables);
            
            new Thread(() => _ = RefreshRoomsThread()).Start();
        });
    }
    
    public JoinRoomMenuViewModel() : base(null!)
    {
        JoinRoomButtonCommand = ReactiveCommand.Create<int>(_ => { });
        NewRoomButtonCommand = NoOpCommand;
        Rooms = Room.GenerateMockRooms(30);
        SelectedRoom = Rooms[0];
    }
    

    private async Task RefreshRoomsThread()
    {
        while (!_isDisposed)
        {            
            var response = await Communicator.Instance.SendRequestAwaitResponse<GetRoomsResponse>(new GetRoomsRequest());

            Rooms = response.Rooms;
            Thread.Sleep(RefreshTime);
        }
    }
}
