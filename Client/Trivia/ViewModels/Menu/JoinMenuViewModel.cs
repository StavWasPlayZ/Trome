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

public class JoinMenuViewModel : PageViewModel, IActivatableViewModel
{
    private const int RefreshTime = 3000;
    
    public ViewModelActivator Activator { get; } = new();
    
    
    public ReactiveCommand<Unit, Unit> NewRoomButtonCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> JoinRoomButtonCommand { get; }


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

    public JoinMenuViewModel(IScreen hostScreen) : base(hostScreen)
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
        
        JoinRoomButtonCommand = NavigateReactiveCommand(
            () => new JoinedRoomViewModel(hostScreen)
        );

        this.WhenActivated(disposables =>
        {
            Disposable
                .Create(() => _isDisposed = true)
                .DisposeWith(disposables);
            
            new Thread(() => _ = RefreshRoomsThread()).Start();
        });
    }
    
    public JoinMenuViewModel() : base(null!)
    {
        JoinRoomButtonCommand = NoOpNavCommand;
        NewRoomButtonCommand = NoOpCommand;
        Rooms = Room.GenerateMockRooms(1);
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
