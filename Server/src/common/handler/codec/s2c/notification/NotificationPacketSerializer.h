#pragma once

#include "Notification.h"
#include "infrastructure/OBuffer.h"

#include <nlohmann/json.hpp>

class NotificationPacketSerializer
{
public:
    // Define as a static class:
    NotificationPacketSerializer() = delete;
    NotificationPacketSerializer(const NotificationPacketSerializer&) = delete;
    NotificationPacketSerializer& operator=(const NotificationPacketSerializer&) = delete;
    NotificationPacketSerializer(NotificationPacketSerializer&&) = delete;
    NotificationPacketSerializer& operator=(NotificationPacketSerializer&&) = delete;


    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const ProtocolNotification& notification);


    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const PlayerJoinedRoomNotification& notification);

    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const PlayerLeftRoomNotification& notification);

    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const RoomClosedNotification& notification);

    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const RoomDataUpdatedNotification& notification);

    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const GameStartedNotification& notification);

    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const GameEndedNotification &notification);

    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const PlayerFinishedNotification &notification);

    /**
     * Converts the provided notification into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(const PlayerKickedNotification &notification);


private:
    static OBuffer serialize(NotificationCode msgCode, const nlohmann::json &data);
};
