using Avalonia.Input;
using Trivia.Models.User;
using Trivia.ViewModels.Popups;
using Trivia.ViewModels.Room;

namespace Trivia.Views.Room;

public abstract class TriviaRushRoomView<TRvm> : RoomView<TRvm> where TRvm : RoomViewModel
{
    protected void UserList_OnPointerReleased(object? sender, PointerReleasedEventArgs pointerReleasedEventArgs)
    {
        Utils.PerformListTriggerAction<RoomUserModel>(sender, OnUserSelected);
    }

    private void OnUserSelected(RoomUserModel userScoreModel)
    {
        if (MainWindowViewModel == null)
            return;

        if ((ViewModel?.IsAdmin != true) || (App.AppService.SessionUser == userScoreModel))
        {
            MainWindowViewModel.PopupContents = new StatsPopupViewModel(userScoreModel);
        }
        else
        {
            MainWindowViewModel.PopupContents = new GuestStatsPopupViewModel(userScoreModel);
        }
    }
}
