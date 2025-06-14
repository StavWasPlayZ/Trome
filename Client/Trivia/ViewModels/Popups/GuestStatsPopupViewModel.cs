using System;
using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Popups;

public class GuestStatsPopupViewModel : StatsPopupViewModel
{
    public ReactiveCommand<Unit, Unit> KickCommand { get; }
    
    public GuestStatsPopupViewModel(User user) : base(user)
    {
        KickCommand = ReactiveCommand.CreateFromTask(async _ =>
        {
            await Comm.SendRequestAsync<KickPlayerResponse>(new KickPlayerRequest(User.Id));
            
            CloseCommand.Execute().Subscribe();
        });
    }
    
    public GuestStatsPopupViewModel()
    {
        KickCommand = NoOpCommand;
    }
}