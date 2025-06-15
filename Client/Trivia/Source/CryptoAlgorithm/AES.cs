using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace Trivia.CryptoAlgorithm;

public class AES : ICryptoAlgorithm
{
    private static readonly Uri KeyPath = ICryptoAlgorithm.UriFor("AESkey.key");
    private static readonly Uri IVPath = ICryptoAlgorithm.UriFor("AESiv.key");
    
    private static byte[] _key = [];
    private static byte[] _iv = [];

    public AES()
    {
        if (_key.Length == 0)
        {
            _key = Utils.ReadAvaresBytes(KeyPath);
        }
        if (_iv.Length == 0)
        {
            _iv = Utils.ReadAvaresBytes(IVPath);
        }
    }
    public string Encrypt(string message)
    {
        using var aes = Aes.Create();
        aes.Key = _key;
        aes.IV = _iv;
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
        aes.Key = _key;
        aes.IV = _iv;
        aes.Mode = CipherMode.CBC;
        aes.Padding = PaddingMode.PKCS7;

        using var decryptor = aes.CreateDecryptor();
        using var ms = new MemoryStream(Convert.FromBase64String(message));
        using var cs = new CryptoStream(ms, decryptor, CryptoStreamMode.Read);
        using var sr = new StreamReader(cs, Encoding.GetEncoding("ISO-8859-1"));

        return sr.ReadToEnd();
    }
}
