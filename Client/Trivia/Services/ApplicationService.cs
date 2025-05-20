using Trivia.Models;

namespace Trivia.Services;

public class ApplicationService
{
    public User? SessionUser { get; set; }
    
    
    public static readonly ApplicationService MockAppService = new()
    {
        SessionUser = new User(1, "Username")
    };
}