using Avalonia.LogicalTree;
using Trivia.ViewModels.Game;

namespace Trivia.Views.Game;

public class RoomViewControl<TPvm> : PageViewControl<TPvm> where TPvm : RoomViewModel
{
    protected void OnAttachedToLogicalTree(object? sender, LogicalTreeAttachmentEventArgs e)
    {
        ViewModel?.SubToServerEvents();
    }
}