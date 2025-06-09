namespace Trivia.CryptoAlgorithm;

public class CipherEncryption : ICryptoAlgorithm
{
    private static readonly int Key = 3;
    public CipherEncryption() { }

    public string Encrypt(string message)
    {
        string encrypted = "";
        foreach (char c in message)
        {
            encrypted += (char)(c + Key);
        }
        
        return encrypted;
    }

    public string Decrypt(string message)
    {
        string decrypted = "";
        foreach (char c in message)
        {
            decrypted += (char)(c - Key);
        }
        
        return decrypted;
    }
}