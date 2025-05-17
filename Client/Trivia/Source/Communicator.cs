using System;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace Trivia;

public class Communicator
{
    public static readonly IPEndPoint DefaultEndpoint = new(IPAddress.Parse("127.0.0.1"), 6942);
    
    public static Communicator Instance { get; } = new();


    private Socket? _clientSocket;
    
    private Communicator()
    {
    }
    

    public async Task Connect(IPEndPoint? endpoint = null)
    {
        endpoint ??= DefaultEndpoint;
        
        Console.WriteLine($"Establishing connection to {endpoint}...");
        
        _clientSocket = new Socket(
            endpoint.AddressFamily,
            SocketType.Stream,
            ProtocolType.Tcp
        );
        
        await _clientSocket.ConnectAsync(endpoint);
        
        Console.WriteLine("Connection successfully established.");
    }
}