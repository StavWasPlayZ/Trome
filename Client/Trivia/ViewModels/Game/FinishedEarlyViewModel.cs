using System.Collections.Generic;
using System.Reactive;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Notification.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Game;

public class FinishedEarlyViewModel : PageViewModel
{
    public FinishedEarlyViewModel(IScreen hostScreen) : base(hostScreen)
    {}
    
    public FinishedEarlyViewModel() : base()
    {}
}