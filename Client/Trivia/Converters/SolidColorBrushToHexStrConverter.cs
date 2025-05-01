using System;
using System.Globalization;
using Avalonia.Data.Converters;
using Avalonia.Media;

namespace Trivia.Converters;

public class SolidColorBrushToHexStrConverter : IValueConverter
{
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not ISolidColorBrush brush)
        {
            return null;
        }

        var color = brush.Color;
        return $"#{color.R:X2}{color.G:X2}{color.B:X2}";
    }

    public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        throw new NotSupportedException();
    }
}