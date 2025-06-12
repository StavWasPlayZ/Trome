using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using Trivia.CryptoAlgorithm;

class RSACrypto : ICryptoAlgorithm
{
    private const string ServerPublicKeyPath = "../../../Source/CryptoAlgorithm/Keys/publicServer.pem";
    private const string ClientPrivateKeyPath = "../../../Source/CryptoAlgorithm/Keys/privateClient.pem";

    private static RSA serverPublic;
    private static RSA clientPrivate;
    
    private static bool KeysLoaded = false;

    public RSACrypto()
    {
        if(!KeysLoaded)
        {
            serverPublic = RSA.Create();
            clientPrivate = RSA.Create();

            if (!File.Exists(ServerPublicKeyPath))
            {
                throw new FileNotFoundException("Server public key file not found.", ServerPublicKeyPath);
            }

            if (!File.Exists(ClientPrivateKeyPath))
            {
                throw new FileNotFoundException("Client private key file not found.", ClientPrivateKeyPath);
            }

            serverPublic.ImportFromPem(File.ReadAllText(ServerPublicKeyPath));
            clientPrivate.ImportFromPem(File.ReadAllText(ClientPrivateKeyPath));
            
            KeysLoaded = true;
        }
    }

    public string Encrypt(string message)
    {
        byte[] data = Encoding.UTF8.GetBytes(message);
        byte[] encrypted = serverPublic.Encrypt(data, RSAEncryptionPadding.OaepSHA1);
        return Convert.ToBase64String(encrypted);
    }

    public string Decrypt(string message)
    {
        byte[] cipherData = Convert.FromBase64String(message);
        byte[] decrypted = clientPrivate.Decrypt(cipherData, RSAEncryptionPadding.OaepSHA1);
        return Encoding.UTF8.GetString(decrypted);
    }
}