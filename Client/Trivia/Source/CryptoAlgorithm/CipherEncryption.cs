namespace Trivia.CryptoAlgorithm;

public class CipherEncryption : ICryptoAlgorithm
{
    private static int key = 3;
    public CipherEncryption() { }

    public string Encrypt(string message)
    {
        string encrypted = "";
        foreach (char c in message)
        {
            encrypted += (char)(c + key);
        }
        
        return encrypted;
    }

    public string Decrypt(string message)
    {
        string decrypted = "";
        foreach (char c in message)
        {
            decrypted += (char)(c - key);
        }
        
        return decrypted;
    }
}