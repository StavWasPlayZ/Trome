using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Avalonia.Threading;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Response;
using Trivia.Exceptions;

namespace Trivia;

public class Communicator : IDisposable
{
    public static readonly IPEndPoint DefaultEndpoint = new(IPAddress.Parse("127.0.0.1"), 6942);
    
    /// <summary>
    /// Whether to print to Debug the various packets.
    /// </summary>
    private const bool Verbose = true;
    
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
    /// Sends the provided request to the server, awaiting a response of <typeparamref name="T"/>.
    /// </summary>
    /// 
    /// <param name="request">
    /// The request to send to the server
    /// </param>
    /// <param name="onResponse">
    /// An action called when the first instance of <typeparamref name="T"/>
    /// is provided by the server
    /// </param>
    /// <param name="onError">
    /// An action called if an <see cref="ErrorResponse"/> was provided instead of <typeparamref name="T"/>.
    /// </param>
    /// 
    /// <typeparam name="T">The expected <see cref="ProtocolResponse"/> type.</typeparam>
    public void SendRequest<T>(ProtocolRequest request, Action<T> onResponse, Action<ErrorResponse>? onError = null)
        where T : ProtocolResponse
    {
        ProtocolResponseReceived += OnProtocolResponseReceived;
        SendRequest(request);
        
        return;

        void OnProtocolResponseReceived(ProtocolResponse response)
        {
            //TODO: Filter out notification packets
            
            if (response is T wantedResponse)
            {
                onResponse(wantedResponse);
            }
            else if (response is ErrorResponse errorResponse)
            {
                onError?.Invoke(errorResponse);
            }
            
            ProtocolResponseReceived -= OnProtocolResponseReceived;
        }
    }

    /// <summary>
    /// Asynchronously sends a request, providing its result.
    /// If an <see cref="ErrorResponse"/> is received, a <see cref="ServerErrorException"/> is thrown.
    /// </summary>
    /// 
    /// <param name="request">The request to send to the server</param>
    /// 
    /// <typeparam name="T">The expected <see cref="ProtocolResponse"/> type</typeparam>
    public async Task<T> SendRequestAwaitResponse<T>(ProtocolRequest request) where T : ProtocolResponse
    {
        var task = new TaskCompletionSource<T>();
        
        SendRequest<T>(
            request,
            response => task.TrySetResult(response),
            response => task.TrySetException(new ServerErrorException(response))
        );
        
        return await task.Task;
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
        
        Log($"Establishing connection to {endpoint}...");
        
        _clientSocket = new TcpClient(endpoint.AddressFamily);
        await _clientSocket.ConnectAsync(endpoint.Address, endpoint.Port);
        
        Log("Connection successfully established.");
        
        new Thread(ListenThread).Start();
        new Thread(WriterThread).Start();

        // TestCommunication();
    }

    
    private void ListenThread()
    {
        while (IsConnected)
        {
            var buffer = new byte[1024];

            int read;
            
            try
            {
                read = _clientSocket!.GetStream().Read(buffer, 0, buffer.Length);
            }
            catch (IOException)
            {
                Console.Error.WriteLine("IO Exception occured; Assuming forced disconnection");
                return;
            }
            
            VerboseLog($"Received packet: {Encoding.UTF8.GetString(buffer, 0, read)}");

            if (read == 0 || !IsConnected)
                return;
            
            var parsed = RequestPacketDeserializer.Deserialize(buffer);

            if (parsed == null)
            {
                Console.Error.WriteLine($"WARNING: Received unknown packet {buffer[0]}.");
                continue;
            }

            VerboseLog($"Successfully parsed as: {parsed}");
            
            // Just dispatch it to the UI thread from here
            Dispatcher.UIThread.Post(() => ProtocolResponseReceived?.Invoke(parsed));
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

            VerboseLog($"Sending packet: {request}");
            VerboseLog($"In raw form: {Encoding.UTF8.GetString(rawRequest, 0, rawRequest.Length)}");
            
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


    [Conditional("DEBUG")]
    private static void VerboseLog(string message)
    {
        if (!Verbose)
            return;
        
        Log(message);
    }
    
    private static void Log(string message)
    {
        Console.WriteLine($"[Communicator] {message}");
    }
}

public delegate void ProtocolResponseHandler(ProtocolResponse request);
