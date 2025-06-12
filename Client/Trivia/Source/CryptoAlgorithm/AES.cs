using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace Trivia.CryptoAlgorithm;

public class AES : ICryptoAlgorithm
{
    private static readonly byte[] Key =
    [
        0x2A, 0x8D, 0xB4, 0x6C, 0xC1, 0xFA, 0x12, 0x34,
        0x7B, 0x9E, 0x56, 0xAF, 0x44, 0xE1, 0x8C, 0x3D
    ];

    private static readonly byte[] IV = 
    [
        0x1F, 0x26, 0x73, 0x9A, 0x4B, 0xD5, 0xCE, 0x22,
        0x39, 0x81, 0x67, 0xF4, 0x0C, 0xAB, 0x58, 0xE7
    ];
    
    public AES()
    {}
    public string Encrypt(string message)
    {
        using var aes = Aes.Create();
        aes.Key = Key;
        aes.IV = IV;
        aes.Mode = CipherMode.CBC;
        aes.Padding = PaddingMode.PKCS7;

        using var encryptor = aes.CreateEncryptor();
        using var ms = new MemoryStream();
        using var cs = new CryptoStream(ms, encryptor, CryptoStreamMode.Write);
        using var sw = new StreamWriter(cs, Encoding.GetEncoding("ISO-8859-1"));

        sw.Write(message);
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