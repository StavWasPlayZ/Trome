using System;
using System.Globalization;
using Avalonia.Data;
using Avalonia.Data.Converters;
using Avalonia.Media;
using Trivia.Codec.C2S;

namespace Trivia.Converters;

public class RoomTypeToBackgroundConverter : IValueConverter
{   
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not RoomType roomType)
        {
            return new BindingNotification(new InvalidCastException(), BindingErrorType.Error);
        }

        return roomType switch
        {
            RoomType.TriviaRush => SolidColorBrush.Parse("#ffb0a1"),
            RoomType.HeadToHead => SolidColorBrush.Parse("#DFABFF"),
            
            _ => throw new ArgumentOutOfRangeException(nameof(value))
        };
    }

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        throw new NotSupportedException();
    }
}