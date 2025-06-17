using System;
using System.IO;

namespace Trivia.CryptoAlgorithm;

public interface ICryptoAlgorithm
{
    private const string KeysPath = "avares://Trivia/Resources/Keys/";
        
    string Encrypt(string message);
    string Decrypt(string message);


    /// <returns>The path (in URI) of the provided key file</returns>
    protected static Uri UriFor(string keyFileName) => new(KeysPath + keyFileName);
    
    
    protected static byte[] ReadFileBytes(string path)
    {
        if (!File.Exists(path))
        {
            throw new FileNotFoundException($"\"{path}\" not found");
        }
        return File.ReadAllBytes(path);
    }
}
