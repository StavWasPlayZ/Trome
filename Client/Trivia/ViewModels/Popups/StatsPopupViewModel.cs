using ReactiveUI;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Popups;

public class StatsPopupViewModel : PopupViewModel
{
    public User User { get; }

    public StatsPopupViewModel(User user) : base(false)
    {
        User = user;
    }
    
    public StatsPopupViewModel() : base(true)
    {
        User = User.CreateMock(1);
        
        _stats = new UserStatistics
        {
            Points = 12,
            GamesPlayed = 12,
            QuestionsAnswered = 12,
            QuestionsAnsweredCorrectly = 12,
            TimeOnQuestionOverall = 12,
            TimeOnQuestionsAvg = -1
        };
    }


    private UserStatistics? _stats;

    public UserStatistics? Stats
    {
        get => _stats;
        private set => this.RaiseAndSetIfChanged(ref _stats, value);
    }
}