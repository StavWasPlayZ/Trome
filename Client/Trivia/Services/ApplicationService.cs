using Trivia.Models.Raw;

namespace Trivia.Services;

public class ApplicationService
{
    public User? SessionUser { get; set; }
    
    
    public static readonly ApplicationService MockAppService = new()
    {
        SessionUser = new User
        {
            Id = 1,
            Username = "Username"
        }
    };
}
