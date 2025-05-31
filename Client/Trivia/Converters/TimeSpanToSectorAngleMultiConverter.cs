using System;
using System.Collections.Generic;
using System.Globalization;
using Avalonia.Data.Converters;

namespace Trivia.Converters;

public class TimeSpanToSectorAngleMultiConverter : IMultiValueConverter
{
    public object? Convert(IList<object?> values, Type targetType, object? parameter, CultureInfo culture)
    {
        if (values.Count < 2)
            return null;

        if (values[0] is not TimeSpan time || values[1] is not int maxTimeSecs)
        {
            return null;
        }

        return (time.TotalMilliseconds / (maxTimeSecs * 1000)) * 360;
    }

    public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        throw new NotSupportedException();
    }
}