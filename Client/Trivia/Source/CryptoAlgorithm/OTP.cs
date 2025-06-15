namespace Trivia.CryptoAlgorithm;

//REVIEW: Consider renaming to Otp
public class OTP : ICryptoAlgorithm
{
    private const string KeyPath = ICryptoAlgorithm.KeysPath + "OTPkey.key";
    
    private static byte[] _key = [];

    public OTP()
    {
        if (_key.Length == 0)
        {
            _key = ICryptoAlgorithm.ReadFileBytes(KeyPath);
        }
    }

    public string Encrypt(string message)
    {
        var encrypted = "";
        var pos = 0;
        
        foreach (var c in message)
        {
            encrypted += (char)(c ^ _key[pos]);
            pos++;
            pos %= _key.Length;
        }
        
        return encrypted;
    }

    public string Decrypt(string message)
    {
        var decrypted = "";
        var pos = 0;
        
        foreach (var c in message)
        {
            decrypted += (char)(c ^ _key[pos]);
            pos++;
            pos %= _key.Length;
        }

        return decrypted;
    }
}
