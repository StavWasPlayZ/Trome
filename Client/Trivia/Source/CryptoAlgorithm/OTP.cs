using System.Security.Cryptography;

namespace Trivia.CryptoAlgorithm;

public class OTP : ICryptoAlgorithm
{
    private readonly string KeyPath = "../../../Source/CryptoAlgorithm/Keys/OTPkey.key";
    private static byte[] Key = [];

    public OTP()
    {
        if (Key.Length == 0)
        {
            Key = ICryptoAlgorithm.ReadFileBytes(KeyPath);
        }
    }

    public string Encrypt(string message)
    {
        string encrypted = "";
        int pos = 0;
        
        foreach (char c in message)
        {
            encrypted += (char)(c ^ Key[pos]);
            pos++;
            pos %= Key.Length;
        }
        
        return encrypted;
    }

    public string Decrypt(string message)
    {
        string decrypted = "";
        int pos = 0;
        
        foreach (char c in message)
        {
            decrypted += (char)(c ^ Key[pos]);
            pos++;
            pos %= Key.Length;
        }

        return decrypted;
    }
}