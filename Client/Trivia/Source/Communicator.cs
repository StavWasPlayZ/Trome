using System;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace Trivia;

public class Communicator : IDisposable
{
    public static readonly IPEndPoint DefaultEndpoint = new(IPAddress.Parse("127.0.0.1"), 6942);
    
    public static Communicator Instance { get; } = new();
    
    private bool _disposed;


    public bool IsConnected => _clientSocket?.Connected ?? false;

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

    
    public void Disconnect()
    {
        if (_clientSocket == null)
            return;

        try
        {
            if (_clientSocket.Connected)
            {
                _clientSocket?.Shutdown(SocketShutdown.Both);
            }
        }
        catch (Exception)
        {
            // ignored
        }
        
        _clientSocket!.Dispose();
        _clientSocket = null;
    }

    public void Dispose()
    {
        if (_disposed)
            return;
        _disposed = true;
        
        Disconnect();

        GC.SuppressFinalize(this);
    }
}
