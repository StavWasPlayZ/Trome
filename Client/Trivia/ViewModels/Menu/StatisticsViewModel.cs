using System.Collections.Generic;
using System.Linq;
using System.Reactive;
using System.Reactive.Disposables;
using System.Threading.Tasks;
using ReactiveUI;
using Trivia.Codec.C2S.Request.Packets;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Controls.Popups;
using Trivia.Models.Raw;
using Trivia.Models.User;

namespace Trivia.ViewModels.Menu;

public class StatisticsViewModel : PageViewModel
{
    private const int Lists = 3;
    private const int MockUsers = 30;
    
    private List<UserScore>? _scores;

    public StatisticsViewModel(IScreen hostScreen) : base(hostScreen)
    {
        this.WhenActivated(disposables =>
        {
            FetchHighScores().DisposeWith(disposables);
        });
    }

    public StatisticsViewModel()
    {
        _scores = Enumerable.Range(1, MockUsers)
            .Select(User.CreateMock)
            .Select(user => new UserScore
            {
                User = user,
                Points = 69420
            })
            .ToList();
        
        GenScoreLists();
    }


    private async Task FetchHighScores()
    {
        var response = await Comm.SendRequestAwaitResponse<GetHighScoresResponse>(new GetHighScoresRequest());

        _scores = [..response.HighScores];
        
        GenScoreLists();
    }

    private void GenScoreLists()
    {
        var scores = new List<UserScoreModel>[Lists];
        
        scores[0] = PickScoreOrNull(0, 2);
        scores[1] = PickScoreOrNull(2, 3);
        
        scores[2] = _scores!.Count > 4
            ? _scores[4..]
                .Select((uScores, i) => new UserScoreModel
                {
                    Place = i + 5,
                    Scores = uScores
                })
                .ToList()
            : [];

        Show2ndList = scores[2].Count != 0;
        ScoreLists = scores;
    }

    private List<UserScoreModel> PickScoreOrNull(int start, int count)
    {
        return Enumerable.Range(start, count)
            .Select(i => new UserScoreModel
            {
                Place = start + i + 1,
                Scores = _scores!.ElementAtOrDefault(i)
            })
            .ToList();
    }
    
    
    private List<UserScoreModel>[] _scoreLists = Enumerable.Range(0, Lists)
        .Select(_ => new List<UserScoreModel>())
        .ToArray();

    public List<UserScoreModel>[] ScoreLists
    {
        get => _scoreLists;
        set => this.RaiseAndSetIfChanged(ref _scoreLists, value);
    }


    // ReSharper disable once InconsistentNaming
    private bool _show2ndList;

    // ReSharper disable once InconsistentNaming
    public bool Show2ndList
    {
        get => _show2ndList;
        set => this.RaiseAndSetIfChanged(ref _show2ndList, value);
    }
    

    public ReactiveCommand<Unit, Unit>? ShowStatsPopup { get; } = ReactiveCommand.Create(() =>
    {
        if (MainWindowViewModel == null)
            return;
        
        MainWindowViewModel.PopupContents = new StatsPopup
        {
            CloseCommand = MainWindowViewModel.CloseDialogCommand
        };
    });
}
