using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Trivia.Codec.C2S.Request;

namespace Trivia;

public class Communicator : IDisposable
{
    public static readonly IPEndPoint DefaultEndpoint = new(IPAddress.Parse("127.0.0.1"), 6942);
    
    public static Communicator Instance { get; } = new();
    
    private bool _disposed;


    public bool IsConnected => _clientSocket?.Connected ?? false;

    private TcpClient? _clientSocket;
    
    private Communicator()
    {
    }
    

    public async Task Connect(IPEndPoint? endpoint = null)
    {
        endpoint ??= DefaultEndpoint;
        
        Console.WriteLine($"Establishing connection to {endpoint}...");
        
        _clientSocket = new TcpClient(endpoint.AddressFamily);
        await _clientSocket.ConnectAsync(endpoint.Address, endpoint.Port);
        
        Console.WriteLine("Connection successfully established.");
        
        new Thread(ListenThread).Start();
        new Thread(WriterThread).Start();
    }

    private void ListenThread()
    {
        while (IsConnected)
        {
            var buffer = new byte[1024];
            var read = _clientSocket!.GetStream().Read(buffer, 0, buffer.Length);

            if (read == 0)
                return;
            
            //TODO: Parse and delegate
            Console.WriteLine("Received: " + Encoding.UTF8.GetString(buffer, 0, read));
        }
    }

    private void WriterThread()
    {
        //TODO: Actually perform writer thread stuff

        var signupRequest = new SignupRequest(
            "c# user",
            "1234",
            "email@example.com",
            "0522222222",
            null,
            "17/06/2008"
        );

        var rawRequest = signupRequest.Serialize();

        _clientSocket!.GetStream().Write(rawRequest, 0, rawRequest.Length);
    }

    
    public void Disconnect()
    {
        _clientSocket?.Dispose();
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
