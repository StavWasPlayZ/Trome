using System.Reactive;
using ReactiveUI;
using Trivia.Services;

namespace Trivia.ViewModels;

public abstract class ViewModelBase(bool isMock) : ReactiveObject, IActivatableViewModel
{
    public ViewModelActivator Activator { get; } = new();
    
    public ApplicationService AppService { get; } = !isMock
        ? App.AppService
        : ApplicationService.MockAppService;

    protected static readonly Communicator Comm = Communicator.Instance;


    protected static MainWindowViewModel? MainWindowViewModel => Utils.MainWindowViewModelInstance;
    
    
    protected static ReactiveCommand<Unit, Unit> NoOpCommand { get; } =
        ReactiveCommand.Create(() => { });
    
    protected static ReactiveCommand<Unit, IRoutableViewModel> NoOpNavCommand { get; } =
        ReactiveCommand.Create(IRoutableViewModel () => null!);
}