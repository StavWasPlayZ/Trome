namespace Trivia.CryptoAlgorithm;

public class NoEncryption : ICryptoAlgorithm
{
    public string Encrypt(string message)
    {
        return message;
    }

    public string Decrypt(string message)
    {
        return message;
    }
}