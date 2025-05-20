using System.Collections.Generic;
using System.Linq;
using System.Reactive;
using System.Reactive.Disposables;
using System.Threading;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C.Response;
using Trivia.Models;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Menu;

public class JoinMenuViewModel : PageViewModel, IActivatableViewModel
{
    public ViewModelActivator Activator { get; } = new();
    
    
    private const int RefreshTime = 3000;

    private static readonly List<Room> MockRooms = Enumerable.Range(1, 30)
        .Select(i => new Room(
            i,
            $"Room {i}",
            i % 2 == 0 ? RoomStatus.Waiting : RoomStatus.Playing,
            new User(i, $"User {i}"),
            10,
            10,
            2,
            10
        ))
        .ToList();
    
    
    public List<Room> Rooms { get; private set; } = [];
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
            
            new Thread(() => _ = RefreshThread()).Start();
        });
    }
    
    public JoinMenuViewModel() : base(null!)
    {
        NewRoomButtonCommand = JoinRoomButtonCommand = NoOpNavCommand;
        Rooms = MockRooms;
    }

    private async Task RefreshThread()
    {
        while (!_isDisposed)
        {
            var response = await Communicator.Instance.SendRequestAwaitResponse<GetRoomsResponse>(new GetRoomsRequest());

            Rooms = response.Rooms;
            Thread.Sleep(RefreshTime);
        }
    }

    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> JoinRoomButtonCommand { get; }
}
