namespace Trivia.CryptoAlgorithm;

//REVIEW: Consider renaming to Otp
public class OTP : ICryptoAlgorithm
{
    private readonly string KeyPath = "../../../Source/CryptoAlgorithm/Keys/OTPkey.key";
    private static byte[] Key = [];

    public OTP()
    {
        if (Key.Length == 0)
        {
            Key = ICryptoAlgorithm.ReadFileBytes(KeyPath);
        }
    }

    public string Encrypt(string message)
    {
        var encrypted = "";
        var pos = 0;
        
        foreach (var c in message)
        {
            encrypted += (char)(c ^ Key[pos]);
            pos++;
            pos %= Key.Length;
        }
        
        return encrypted;
    }

    public string Decrypt(string message)
    {
        var decrypted = "";
        var pos = 0;
        
        foreach (var c in message)
        {
            decrypted += (char)(c ^ Key[pos]);
            pos++;
            pos %= Key.Length;
        }

        return decrypted;
    }
}