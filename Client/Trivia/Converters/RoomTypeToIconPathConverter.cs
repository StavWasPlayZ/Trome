using System;
using System.Globalization;
using Avalonia.Data;
using Avalonia.Data.Converters;
using Trivia.Codec.C2S;

namespace Trivia.Converters;

public class RoomTypeToIconPathConverter : IValueConverter
{   
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        if (value is not RoomType roomType)
        {
            return new BindingNotification(new InvalidCastException(), BindingErrorType.Error);
        }

        return roomType switch
        {
            RoomType.TriviaRush => "avares://Trivia/Assets/Icons/sports_kabaddi.svg",
            RoomType.HeadToHead => "avares://Trivia/Assets/Icons/group.svg",
            
            _ => throw new ArgumentOutOfRangeException(nameof(value))
        };
    }

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        throw new NotSupportedException();
    }
}