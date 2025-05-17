using System;
using System.Globalization;
using Avalonia.Data;
using Avalonia.Data.Converters;

namespace Trivia.Converters;

/**
 * Performs simple multiplication for the provided value
 */
public class MultiplicationConverter : IValueConverter
{
    public static readonly MultiplicationConverter Instance = new();
    
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not double val || parameter is not string param)
        {
            Console.WriteLine(parameter?.GetType());
            return new BindingNotification(new InvalidCastException(), BindingErrorType.Error);
        }
        
        return val * double.Parse(param);
    }

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not double val || parameter is not double param)
        {
            return new BindingNotification(new InvalidCastException(), BindingErrorType.Error);
        }
        
        return val / param;
    }
}