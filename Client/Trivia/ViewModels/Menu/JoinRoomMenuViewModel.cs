using System;
using System.Collections.Generic;
using System.Reactive;
using System.Reactive.Disposables;
using System.Threading.Tasks;
using Avalonia.Threading;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Exceptions;
using Trivia.Models;
using Trivia.ViewModels.Room;

namespace Trivia.ViewModels.Menu;

public class JoinRoomMenuViewModel : PageViewModel
{
    private static readonly TimeSpan RefreshTime = TimeSpan.FromSeconds(3);
    
    
    public ReactiveCommand<Unit, IRoutableViewModel> NewRoomButtonCommand { get; }
    
    public ReactiveCommand<int, Unit> JoinRoomButtonCommand { get; }

    
    private readonly DispatcherTimer? _roomFetcher;
    

    private List<RoomModel> _rooms = [];

    public List<RoomModel> Rooms
    {
        get => _rooms;
        private set => this.RaiseAndSetIfChanged(ref _rooms, value);
    }

    private RoomModel? _room;
    
    public RoomModel? SelectedRoom
    {
        get => _room;
        set => this.RaiseAndSetIfChanged(ref _room, value);
    }
    

    public JoinRoomMenuViewModel(IScreen hostScreen) : base(hostScreen)
    {
        NewRoomButtonCommand = NavigateReactiveCommand(() => new RoomTypeSelectorViewModel());

        JoinRoomButtonCommand = ReactiveCommand.CreateFromTask<int>(JoinRoom);

        _roomFetcher = new DispatcherTimer
        {
            Interval = RefreshTime
        };
        
        this.WhenActivated(disposables =>
        {
            _roomFetcher.Tick += RefreshRooms;
            _roomFetcher.Start();

            RefreshRooms().DisposeWith(disposables);
            
            Disposable
                .Create(() =>
                {
                    _roomFetcher.Tick -= RefreshRooms;
                    _roomFetcher.Stop();
                })
                .DisposeWith(disposables);
            
            JoinRoomButtonCommand
                .ThrownExceptions
                .Subscribe(OnJoinRoomFailed)
                .DisposeWith(disposables);
        });
    }

    public JoinRoomMenuViewModel() : base(null!)
    {
        JoinRoomButtonCommand = ReactiveCommand.Create<int>(_ => { });
        NewRoomButtonCommand = NoOpNavCommand;
        Rooms = RoomModel.GenerateMockRooms(30);
        SelectedRoom = Rooms[0];
    }

    private async Task JoinRoom(int roomId)
    {
        //TODO: Handle room deleted before refresh
        var response = await Comm.SendRequestAwaitResponse<JoinRoomResponse>(new JoinRoomRequest(roomId));
        
        NavigateTo(new RoomGuestViewModel(HostScreen, response.Room, [..response.Players]));
    }

    private void OnJoinRoomFailed(Exception exception)
    {
        if (exception is not ServerErrorException e)
            throw exception;

        switch (e.ServerResponse.Status)
        {
            case ErrorStatus.RoomFull:
            case ErrorStatus.RoomAlreadyPlaying:
                return;
            
            default:
                throw exception;
        }
    }


    private void RefreshRooms(object? sender, EventArgs e) => _ = RefreshRooms();

    private async Task RefreshRooms()
    {
        var response = await Communicator.Instance.SendRequestAwaitResponse<GetRoomsResponse>(new GetRoomsRequest());
        Rooms = [..response.Rooms];
    }
}
