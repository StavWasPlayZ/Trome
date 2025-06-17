using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using Avalonia.Platform;

namespace Trivia.CryptoAlgorithm;

public class RsaCrypto : ICryptoAlgorithm
{
    private static readonly Uri ServerPublicKeyPath = ICryptoAlgorithm.UriFor("publicServer.pem");
    private static readonly Uri ClientPrivateKeyPath = ICryptoAlgorithm.UriFor("privateClient.pem");

    private static RSA _serverPublic = null!;
    private static RSA _clientPrivate = null!;

    private static bool _keysLoaded;
    private static int _keySizeInBytes;
    private static int _maxDataLength;

    public RsaCrypto()
    {
        if (!_keysLoaded)
        {
            LoadKeys();
        }
    }

    private static void LoadKeys()
    {
        _serverPublic = RSA.Create();
        _clientPrivate = RSA.Create();

        if (!AssetLoader.Exists(ServerPublicKeyPath))
            throw new FileNotFoundException("Server public key file not found.", ServerPublicKeyPath.AbsolutePath);

        if (!AssetLoader.Exists(ClientPrivateKeyPath))
            throw new FileNotFoundException("Client private key file not found.", ClientPrivateKeyPath.AbsolutePath);

        _serverPublic.ImportFromPem(Utils.ReadAvares(ServerPublicKeyPath));
        _clientPrivate.ImportFromPem(Utils.ReadAvares(ClientPrivateKeyPath));

        _keySizeInBytes = _serverPublic.KeySize / 8; // e.g. 1024 bytes for 8192 bits
        _maxDataLength = _keySizeInBytes - 42; // for OAEP-SHA1 padding

        _keysLoaded = true;
    }
    

    public string Encrypt(string message)
    {
        var data = Encoding.UTF8.GetBytes(message);
        var encryptedData = EncryptInChunks(data, _serverPublic);
        return Convert.ToBase64String(encryptedData);
    }

    public string Decrypt(string base64Cipher)
    {
        var cipherData = Convert.FromBase64String(base64Cipher);
        var decryptedData = DecryptInChunks(cipherData, _clientPrivate);
        return Encoding.UTF8.GetString(decryptedData);
    }

    private static byte[] EncryptInChunks(byte[] data, RSA rsa)
    {
        using var plainStream = new MemoryStream(data);
        using var encryptedStream = new MemoryStream();
        
        var buffer = new byte[_maxDataLength];
        int bytesRead;

        while ((bytesRead = plainStream.Read(buffer, 0, _maxDataLength)) > 0)
        {
            var toEncrypt = buffer;
            if (bytesRead != _maxDataLength)
            {
                toEncrypt = new byte[bytesRead];
                Array.Copy(buffer, toEncrypt, bytesRead);
            }

            var encryptedChunk = rsa.Encrypt(toEncrypt, RSAEncryptionPadding.OaepSHA1);
            encryptedStream.Write(encryptedChunk, 0, encryptedChunk.Length);
        }

        return encryptedStream.ToArray();
    }

    private byte[] DecryptInChunks(byte[] encryptedData, RSA rsa)
    {
        using var encryptedStream = new MemoryStream(encryptedData);
        using var decryptedStream = new MemoryStream();
        
        var buffer = new byte[_keySizeInBytes];
        int bytesRead;

        while ((bytesRead = encryptedStream.Read(buffer, 0, _keySizeInBytes)) > 0)
        {
            if (bytesRead != _keySizeInBytes)
                throw new Exception("Invalid encrypted chunk size during decryption.");

            var decryptedChunk = rsa.Decrypt(buffer, RSAEncryptionPadding.OaepSHA1);
            decryptedStream.Write(decryptedChunk, 0, decryptedChunk.Length);
        }

        return decryptedStream.ToArray();
    }
}