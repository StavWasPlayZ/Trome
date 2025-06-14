using System.Reactive.Disposables;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Models.Raw;

namespace Trivia.ViewModels.Popups;

public class StatsPopupViewModel : PopupViewModel
{
    public User User { get; }

    public StatsPopupViewModel(User user) : base(false)
    {
        User = user;
        
        this.WhenActivated(disposables =>
        {
            FetchUserStatistics()
                .DisposeWith(disposables);
        });
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


    private async Task FetchUserStatistics()
    {
        var response = await Comm.SendRequestAsync<GetUserStatisticsResponse>(
            new GetUserStatisticsRequest(User.Id)
        );
        
        Stats = response.Stats;
    }


    private UserStatistics? _stats;

    public UserStatistics? Stats
    {
        get => _stats;
        private set => this.RaiseAndSetIfChanged(ref _stats, value);
    }
}