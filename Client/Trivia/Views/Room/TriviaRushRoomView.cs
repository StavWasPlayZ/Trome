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

    private static void OnUserSelected(RoomUserModel userScoreModel)
    {
        if (MainWindowViewModel == null)
            return;

        MainWindowViewModel.PopupContents = new StatsPopupViewModel(userScoreModel);
    }
}
