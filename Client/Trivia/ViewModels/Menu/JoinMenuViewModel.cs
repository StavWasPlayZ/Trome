using System.Collections.Generic;
using System.Linq;
using System.Reactive;
using System.Reactive.Disposables;
using System.Threading;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C.Response;
using Trivia.Models.Raw;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Menu;

public class JoinMenuViewModel : PageViewModel, IActivatableViewModel
{
    public ViewModelActivator Activator { get; } = new();
    
    
    private const int RefreshTime = 3000;

    private static readonly List<Room> MockRooms = Enumerable.Range(1, 30)
        .Select(i => new Room
            {
                Id = i,
                Status = i % 2 == 0 ? RoomStatus.Waiting : RoomStatus.Playing,
                Admin = new User
                {
                    Id = i,
                    Username = $"User {i}"
                },
                
                PlayersCount = 2,
                
                Data = new RoomData
                {
                    Name = $"Room {i}",
                    MaxPlayers = 10,
                    TimePerQuestionSecs = 7,
                    QuestionsCount = 15
                }
            }
        ).ToList();
    
    
    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; }
    
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
        NewRoomButtonCommand = NavigateReactiveCommand(
            () => new CreateRoomViewModel(hostScreen)
        );
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
        NewRoomButtonCommand = JoinRoomButtonCommand = NoOpNavCommand;
        Rooms = MockRooms;
        SelectedRoom = MockRooms[0];
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
