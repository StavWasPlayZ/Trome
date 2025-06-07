using System;
using System.Globalization;
using Avalonia.Data;
using Avalonia.Data.Converters;

namespace Trivia.Converters;

public class InvalidStaterConverter : IValueConverter
{   
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not int val)
        {
            return new BindingNotification(new InvalidCastException(), BindingErrorType.Error);
        }

        return val < 0 ? "----" : val;
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