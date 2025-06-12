using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace Trivia.CryptoAlgorithm;

public class AES : ICryptoAlgorithm
{
    private static readonly string KeyPath = "../../../Source/CryptoAlgorithm/Keys/AESkey.key";
    private static readonly string IVPath = "../../../Source/CryptoAlgorithm/Keys/AESiv.key";
    
    private static readonly byte[] Key = ICryptoAlgorithm.ReadFileBytes(KeyPath);
    private static readonly byte[] IV = ICryptoAlgorithm.ReadFileBytes(IVPath);

    public AES()
    {
        throw new FileNotFoundException("AES key not found");
    }
    public string Encrypt(string message)
    {
        using var aes = Aes.Create();
        aes.Key = Key;
        aes.IV = IV;
        aes.Mode = CipherMode.CBC;
        aes.Padding = PaddingMode.PKCS7;

        using var encryptor = aes.CreateEncryptor();
        using var ms = new MemoryStream();
        using (var cs = new CryptoStream(ms, encryptor, CryptoStreamMode.Write))
        using (var sw = new StreamWriter(cs, Encoding.GetEncoding("ISO-8859-1")))
        {
            sw.Write(message);
        }

        return Convert.ToBase64String(ms.ToArray());
    }

    public string Decrypt(string message)
    {
        using var aes = Aes.Create();
        aes.Key = Key;
        aes.IV = IV;
        aes.Mode = CipherMode.CBC;
        aes.Padding = PaddingMode.PKCS7;

        using var decryptor = aes.CreateDecryptor();
        using var ms = new MemoryStream(Convert.FromBase64String(message));
        using var cs = new CryptoStream(ms, decryptor, CryptoStreamMode.Read);
        using var sr = new StreamReader(cs, Encoding.GetEncoding("ISO-8859-1"));

        return sr.ReadToEnd();
    }

}