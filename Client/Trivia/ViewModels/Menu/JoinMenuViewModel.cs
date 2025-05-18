using System.Collections.Generic;
using System.Reactive;
using System.Reactive.Disposables;
using ReactiveUI;
using Trivia.Models;
using Trivia.ViewModels.Game;

namespace Trivia.ViewModels.Menu;

public class JoinMenuViewModel : PageViewModel, IActivatableViewModel
{
    public ViewModelActivator Activator { get; } = new();
    
    private List<Room> rooms = [];

    public JoinMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        NewRoomButtonCommand = NavigateReactiveCommand(
            () => new CreateRoomViewModel(hostScreen)
        );
        JoinRoomButtonCommand = NavigateReactiveCommand(
            () => new JoinedRoomViewModel(hostScreen)
        );

        this.WhenActivated((CompositeDisposable _) =>
        {
            //TODO: Create thread to refresh rooms
        });
    }


    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; }
    
    public ReactiveCommand<Unit, IRoutableViewModel> JoinRoomButtonCommand { get; }
}
