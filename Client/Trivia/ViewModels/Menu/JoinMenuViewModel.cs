using System;
using System.Collections.Generic;
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
    
    private List<Room> rooms = [];
    private bool _isDisposed;
    private readonly int refreshTime = 3000;

    public JoinMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        NewRoomButtonCommand = NavigateReactiveCommand(
            () => new CreateRoomViewModel(hostScreen)
        );
        JoinRoomButtonCommand = NavigateReactiveCommand(
            () => new JoinedRoomViewModel(hostScreen)
        );

        this.WhenActivated((CompositeDisposable disposables) =>
        {
            //TODO: Create thread to refresh rooms
            Disposable
                .Create(() => _isDisposed = true)
                .DisposeWith(disposables);

            _ = RefreshThread();
        });
    }

    private async Task RefreshThread()
    {
        while (!_isDisposed)
        {
            var response = await Communicator.Instance.SendRequestAwaitResponse<GetRoomsResponse>(new GetRoomsRequest());

            rooms = response.Rooms;
            Thread.Sleep(refreshTime);
        }
    }

    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> JoinRoomButtonCommand { get; }
}
