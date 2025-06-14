using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Avalonia.Controls;
using Avalonia.Threading;
using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C;
using Trivia.Codec.S2C.Response.Packets;
using Trivia.Codec.S2C.Response.Packets.Impl;
using Trivia.Exceptions;

namespace Trivia;

public class Communicator : IDisposable
{
    public static readonly IPEndPoint DefaultEndpoint = new(IPAddress.Parse("127.0.0.1"), 6942);
    
    /// <summary>
    /// Whether to print to Debug the various packets.
    /// </summary>
    public const bool Verbose = true;
    
    public static Communicator Instance { get; } = new();
    
    private bool _disposed;
    
    
    public enum LogLevel { Info, Warning, Sending, Receiving }
    public static class AnsiColor
    {
        public const string Reset = "\u001b[0m";
        public const string White = "\u001b[37m";
        public const string DarkYellow = "\u001b[33m";
        public const string Blue = "\u001b[34m";
        public const string DarkCyan = "\u001b[36m";
    }
    
    private readonly Queue<IProtocolRequest> _outgoingRequests = [];
    private readonly object _outgoingRequestsCv = new();
    
    /// <summary>
    /// Use this to subscribe to new server packets received.
    /// </summary>
    public event ServerPacketHandler? PacketReceived;


    public bool IsConnected => _clientSocket?.Connected ?? false;

    private TcpClient? _clientSocket;
    
    private Communicator() { }


    /// <summary>
    /// Sends the provided request to the server.
    /// </summary>
    /// <param name="request">The request to send to the server</param>
    public void SendRequest(IProtocolRequest request)
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
    /// <typeparam name="T">The expected <see cref="IS2CPacket"/> type.</typeparam>
    public void SendRequest<T>(IProtocolRequest request, Action<T> onResponse, Action<ErrorResponse>? onError = null)
        where T : IProtocolResponse
    {
        PacketReceived += OnPacketReceived;
        SendRequest(request);
        
        return;

        void OnPacketReceived(IS2CPacket packet)
        {
            if (packet is not T wantedResponse)
            {
                if (packet is ErrorResponse errorResponse)
                {
                    //TODO: Check if it actually corresponds to the original code
                    onError?.Invoke(errorResponse);
                    PacketReceived -= OnPacketReceived;
                }

                return;
            }

            onResponse(wantedResponse);
            PacketReceived -= OnPacketReceived;
        }
    }

    /// <summary>
    /// Asynchronously sends a request, providing its result.
    /// If an <see cref="ErrorResponse"/> is received, a <see cref="ServerErrorException"/> is thrown.
    /// </summary>
    /// 
    /// <param name="request">The request to send to the server</param>
    /// 
    /// <typeparam name="T">The expected <see cref="IS2CPacket"/> type</typeparam>
    public async Task<T> SendRequestAsync<T>(IProtocolRequest request) where T : IProtocolResponse
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
        // Just to make sure
        if (Design.IsDesignMode)
            return;
        
        endpoint ??= DefaultEndpoint;
        
        Log($"Establishing connection to {endpoint}...");
        
        _clientSocket = new TcpClient(endpoint.AddressFamily);
        await _clientSocket.ConnectAsync(endpoint.Address, endpoint.Port);
        
        Log("Connection successfully established.");
        
        new Thread(ListenThread).Start();
        new Thread(WriterThread).Start();
    }

    
    private void ListenThread()
    {
        while (IsConnected)
        {
            IS2CPacket? serverPacket;
            
            try
            {
                serverPacket = ReadServerPacket();
            }
            catch (IOException e)
            {
                if (e.Message.Contains("forcibly closed by the remote host"))
                {
                    VerboseLog("Server disconnected; Closing the game", LogLevel.Warning);
                }
                else if (e.Message.Contains("connection was aborted by the software in your host machine"))
                {
                    VerboseLog("Successfully disconnected");
                }
                else
                {
                    Console.Error.WriteLine($"IO Exception occured ({e.Message}); Assuming forced disconnection");   
                }
                
                Dispatcher.UIThread.Post(MainWindow.Instance!.Close);
                return;
            }

            if (serverPacket == null)
                return;
            
            // Just dispatch it to the UI thread from here
            Dispatcher.UIThread.Post(() => PacketReceived?.Invoke(serverPacket));
        }
    }

    private IS2CPacket? ReadServerPacket()
    {
        var packetType = ReadSingleByte();

        if (packetType == null)
            return null;
        
        
        var code = ReadSingleByte();

        if (code == null)
            return null;

        
        var jsonLengthRaw = new byte[sizeof(int)];
        var read = _clientSocket!.GetStream().Read(jsonLengthRaw, 0, jsonLengthRaw.Length);
        
        if (read == 0 || !IsConnected)
            return null;
        
        if (BitConverter.IsLittleEndian)
        {
            Array.Reverse(jsonLengthRaw);
        }
        
        var jsonLength = BitConverter.ToInt32(jsonLengthRaw, 0);
        
        
        var jsonRaw = new byte[jsonLength];
        read = _clientSocket!.GetStream().Read(jsonRaw, 0, jsonRaw.Length);
        
        if (read == 0 || !IsConnected)
            return null;
        
        // TODO: Decrypt here
        
        var json = Encoding.UTF8.GetString(jsonRaw, 0, jsonRaw.Length);

        
        return PacketDeserializer.Deserialize((S2CPacketType) packetType, (byte) code, json);
    }

    private byte? ReadSingleByte()
    {
        var result = new byte[1];
        var read = _clientSocket!.GetStream().Read(result, 0, result.Length);

        if (read == 0 || !IsConnected)
            return null;
        
        return result[0];
    }
    

    private void WriterThread()
    {
        while (IsConnected)
        {
            IProtocolRequest request;
            
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

            VerboseLog($"Sending packet: {request}", LogLevel.Sending);
            VerboseLog($"In raw form: {Encoding.UTF8.GetString(rawRequest, 5, rawRequest.Length - 5)}", LogLevel.Sending);
            
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
    private static void VerboseLog(string message, LogLevel level = LogLevel.Info)
    {
        if (!Verbose)
            return;
        
        Log(message, level);
    }
    

    private static void Log(string message, LogLevel level = LogLevel.Info)
    {
        string color = level switch
        {
            LogLevel.Info => AnsiColor.Reset,
            LogLevel.Warning => AnsiColor.DarkYellow,
            LogLevel.Sending => AnsiColor.Blue,
            LogLevel.Receiving => AnsiColor.DarkCyan,
            _ => AnsiColor.Reset
        };

        Console.WriteLine($"{color}[{nameof(Communicator)}] {message}{AnsiColor.Reset}");
    }
}

public delegate void ServerPacketHandler(IS2CPacket packet);
