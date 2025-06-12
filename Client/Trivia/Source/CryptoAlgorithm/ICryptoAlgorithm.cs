using System.IO;

namespace Trivia.CryptoAlgorithm;

public interface ICryptoAlgorithm
{
    string Encrypt(string message);
    string Decrypt(string message);
    
    protected static byte[] ReadFileBytes(string path)
    {
        if (!File.Exists(path))
        {
            throw new FileNotFoundException($"\"{path}\" not found");
        }
        return File.ReadAllBytes(path);
    }
}
