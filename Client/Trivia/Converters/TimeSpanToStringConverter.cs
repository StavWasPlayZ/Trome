using System;
using System.Globalization;
using Avalonia.Data.Converters;

namespace Trivia.Converters;

public class TimeSpanToStringConverter : IValueConverter
{
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not TimeSpan time)
        {
            return null;
        }

        return time.ToString("m\\:ss");
    }

    public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        throw new NotSupportedException();
    }
}