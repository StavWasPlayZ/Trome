namespace Trivia.CryptoAlgorithm;

public interface ICryptoAlgorithm
{
    string Encrypt(string message);
    string Decrypt(string message);
}
