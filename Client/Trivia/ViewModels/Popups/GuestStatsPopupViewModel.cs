using Trivia.Models.Raw;

namespace Trivia.ViewModels.Popups;

public class GuestStatsPopupViewModel : StatsPopupViewModel
{
    public GuestStatsPopupViewModel(User user) : base(user)
    {
    }
    
    public GuestStatsPopupViewModel()
    {
    }
}