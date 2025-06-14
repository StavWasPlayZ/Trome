using Trivia.ViewModels.Room;

namespace Trivia.Views.Room;

public abstract class RoomView<TRvm> : PageViewControl<TRvm> where TRvm : RoomViewModel;
