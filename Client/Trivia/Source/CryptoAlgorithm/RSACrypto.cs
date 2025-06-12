using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using Trivia.CryptoAlgorithm;

public class RSACrypto : ICryptoAlgorithm
{
    private const string ServerPublicKeyPath = "../../../Source/CryptoAlgorithm/Keys/publicServer.pem";
    private const string ClientPrivateKeyPath = "../../../Source/CryptoAlgorithm/Keys/privateClient.pem";

    private static RSA serverPublic;
    private static RSA clientPrivate;

    private static bool KeysLoaded = false;
    private static int keySizeInBytes;
    private static int maxDataLength;

    public RSACrypto()
    {
        if (!KeysLoaded)
        {
            serverPublic = RSA.Create();
            clientPrivate = RSA.Create();

            if (!File.Exists(ServerPublicKeyPath))
                throw new FileNotFoundException("Server public key file not found.", ServerPublicKeyPath);

            if (!File.Exists(ClientPrivateKeyPath))
                throw new FileNotFoundException("Client private key file not found.", ClientPrivateKeyPath);

            serverPublic.ImportFromPem(File.ReadAllText(ServerPublicKeyPath));
            clientPrivate.ImportFromPem(File.ReadAllText(ClientPrivateKeyPath));

            keySizeInBytes = serverPublic.KeySize / 8; // e.g. 1024 bytes for 8192 bits
            maxDataLength = keySizeInBytes - 42; // for OAEP-SHA1 padding

            KeysLoaded = true;
        }
    }

    public string Encrypt(string message)
    {
        byte[] data = Encoding.UTF8.GetBytes(message);
        byte[] encryptedData = EncryptInChunks(data, serverPublic);
        return Convert.ToBase64String(encryptedData);
    }

    public string Decrypt(string base64Cipher)
    {
        byte[] cipherData = Convert.FromBase64String(base64Cipher);
        byte[] decryptedData = DecryptInChunks(cipherData, clientPrivate);
        return Encoding.UTF8.GetString(decryptedData);
    }

    private byte[] EncryptInChunks(byte[] data, RSA rsa)
    {
        using (MemoryStream plainStream = new MemoryStream(data))
        using (MemoryStream encryptedStream = new MemoryStream())
        {
            byte[] buffer = new byte[maxDataLength];
            int bytesRead;

            while ((bytesRead = plainStream.Read(buffer, 0, maxDataLength)) > 0)
            {
                byte[] toEncrypt = buffer;
                if (bytesRead != maxDataLength)
                {
                    toEncrypt = new byte[bytesRead];
                    Array.Copy(buffer, toEncrypt, bytesRead);
                }

                byte[] encryptedChunk = rsa.Encrypt(toEncrypt, RSAEncryptionPadding.OaepSHA1);
                encryptedStream.Write(encryptedChunk, 0, encryptedChunk.Length);
            }

            return encryptedStream.ToArray();
        }
    }

    private byte[] DecryptInChunks(byte[] encryptedData, RSA rsa)
    {
        using (MemoryStream encryptedStream = new MemoryStream(encryptedData))
        using (MemoryStream decryptedStream = new MemoryStream())
        {
            byte[] buffer = new byte[keySizeInBytes];
            int bytesRead;

            while ((bytesRead = encryptedStream.Read(buffer, 0, keySizeInBytes)) > 0)
            {
                if (bytesRead != keySizeInBytes)
                    throw new Exception("Invalid encrypted chunk size during decryption.");

                byte[] decryptedChunk = rsa.Decrypt(buffer, RSAEncryptionPadding.OaepSHA1);
                decryptedStream.Write(decryptedChunk, 0, decryptedChunk.Length);
            }

            return decryptedStream.ToArray();
        }
    }
}