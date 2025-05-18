using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Response;

namespace Trivia;

public class Communicator : IDisposable
{
    public static readonly IPEndPoint DefaultEndpoint = new(IPAddress.Parse("127.0.0.1"), 6942);
    
    public static Communicator Instance { get; } = new();
    
    private bool _disposed;
    
    
    private readonly Queue<ProtocolRequest> _outgoingRequests = [];
    private readonly object _outgoingRequestsCv = new();
    
    /// <summary>
    /// Use this to subscribe to new server packets received.
    /// </summary>
    public event ProtocolResponseHandler? ProtocolResponseReceived;


    public bool IsConnected => _clientSocket?.Connected ?? false;

    private TcpClient? _clientSocket;
    
    private Communicator() { }


    /// <summary>
    /// Sends the provided request to the server.
    /// </summary>
    /// <param name="request">The request to send to the server</param>
    public void SendRequest(ProtocolRequest request)
    {
        lock (_outgoingRequests)
        {
            _outgoingRequests.Enqueue(request);
        }
        
        NotifyNewOutgoingRequest();
    }

    /// <summary>
    /// Wakes up the Packet Writer thread to write
    /// any new entries of _outgoingRequests.
    /// </summary>
    private void NotifyNewOutgoingRequest()
    {
        lock (_outgoingRequestsCv)
        {
            Monitor.Pulse(_outgoingRequestsCv);
        }
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

        // TestCommunication();
    }

    // private void TestCommunication()
    // {
    //     SendRequest(new SignupRequest(
    //         "c# user",
    //         "1234",
    //         "email@example.com",
    //         "0522222222",
    //         null,
    //         "17/06/2008"
    //     ));
    //     
    //     ProtocolResponseReceived += response =>
    //     {
    //         return;
    //     };
    // }

        
    private void ListenThread()
    {
        while (IsConnected)
        {
            var buffer = new byte[1024];
            var read = _clientSocket!.GetStream().Read(buffer, 0, buffer.Length);

            if (read == 0)
                return;
            
            var parsed = RequestPacketDeserializer.Deserialize(buffer);

            if (parsed == null)
            {
                Console.Error.WriteLine($"WARNING: Received unknown packet {buffer[0]}.");
                continue;
            }
            
            ProtocolResponseReceived?.Invoke(parsed);
        }
    }

    private void WriterThread()
    {
        while (IsConnected)
        {
            ProtocolRequest request;
            
            lock (_outgoingRequestsCv)
            {
                while (_outgoingRequests.Count == 0)
                {
                    Monitor.Wait(_outgoingRequestsCv);

                    if (!IsConnected)
                        return;
                }
                
                request = _outgoingRequests.Dequeue();
            }
            
            var rawRequest = request.Serialize();
            _clientSocket!.GetStream().Write(rawRequest, 0, rawRequest.Length);
        }
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
        NotifyNewOutgoingRequest();

        GC.SuppressFinalize(this);
    }
}

public delegate void ProtocolResponseHandler(IProtocolResponse request);
