using System;
using System.Globalization;
using Avalonia.Data.Converters;

namespace Trivia.Converters;

public class SecondsToTimeStringConverter : IValueConverter
{
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not int secs)
        {
            return value;
        }

        if (secs < 0)
        {
            return "----";
        }

        return TimeSpan.FromSeconds(secs).ToString("m\\:ss");
    }

    public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        throw new NotSupportedException();
    }
}