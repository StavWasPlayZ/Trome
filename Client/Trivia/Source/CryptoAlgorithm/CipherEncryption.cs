using System.Linq;

namespace Trivia.CryptoAlgorithm;

public class CipherEncryption : ICryptoAlgorithm
{
    private const int Key = 3;

    public string Encrypt(string message)
    {
        return message
            .Aggregate("", (current, c) =>
                current + (char)(c + Key)
            );
    }

    public string Decrypt(string message)
    {
        return message
            .Aggregate("", (current, c) =>
                current + (char)(c - Key)
            );
    }
}