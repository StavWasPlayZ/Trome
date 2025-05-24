#pragma once

#include <optional>
#include <string>

class Room;
class Client;

class LoggedUser
{
public:
    LoggedUser(unsigned int id, const std::string& username, Client& client);

    const std::string& getUsername() const;
    unsigned int getId() const;
    bool operator==(const LoggedUser &other) const;

    Client& getClient() const;


    void setCurrentRoom(Room& room);
    std::optional<Room*> getCurrentRoom() const;

    void removeFromRoom();


    void handleDisconnecting();
    
private:
    Client& m_client;

    const unsigned int m_id;
    std::string m_username;

    Room* m_currentRoom;
};
