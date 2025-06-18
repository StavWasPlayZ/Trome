# TROME

<img src="./public/assets/banner.png" alt="Banner" width="700"/>

Trome was created and submitted as part of the Magshimim National Cyber Program 2nd year final project.

Made by Itay Herskovits and Stav Solomon.

# Features

- Cross-platform server & client
- 2 Gamemodes
    - Trivia Rush
    - Head-to-Head
- User-defined questions
- Player kicking
- BGM !!

## Bonuses
Features explicitly deemed as a bonus by Magshimim, as well as additional, custom ones we simply wanted to add:

### Official
- Regexes
    - Signup validation with regexes using the [Compile Time Regular Expressions](https://github.com/hanickadot/compile-time-regular-expressions) library
- Singletons
    - The transformation of the following classes to use the Singleton pattern:
        - `Server`
        - `Database`
        - `CommonCommunicator`
        - `WindowsCommunicator`
        - `UnixCommunicator`
    - The transformation of the following classes to use the Static Class pattern:
        - `JsonRequestPacketDeserializer`
        - `JsonResponsePacketSerializer`
- MongoDB
- Pulling questions from [OpenTDB](https://opentdb.com/) into the database
- User-defined questions
- Head-to-Head gamemode
    - Pre-game countdown
- Cryptography
    - OTP
    - Crypto++
        - RSA
        - AES

### Custom
- CMake-aligned project
- Cross-platform support for Windows/Unix
- Notifications system
- Avalonia
- Harmonically syncing BGM w/ [NAudio](https://github.com/naudio/NAudio)
- Viewing player statistics in-game
- Player kicking
- SQLite Prepared Statements & Bindings

# Installation & Setup

# Server Infrastructure

## State Management

### 📍 States

| State Name              | Purpose                        | Description                                                                                                                                                                                                                                     |
|-------------------------|--------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `Login`                 | Initial state / Authentication | The client has just connected and is not yet aligned with a user                                                                                                                                                                                |
| `Menu`                  | Menu                           | The main hub for all primary actions, such as joining a room, viewing statistics, etc.                                                                                                                                                          |
| `Room Member`           | Waiting room                   | The waiting state for all room guests for when until the room admin wishes to start the game. The user may request to see the room data and the statistics of the users that are in the room. Leaving the room will notify everyone else in it. |
| `Room Admin`            | Waiting room                   | Same as the `Room Member` State, but may also perform admin actions; kick players, change the room data and start the game. As an admin, leaving the room will close it & notify others.                                                        |
| `Game`                  | Playing the game               | The actual game state. Can request questions and submit answers. Leaving it prompts the same behavior as `Room Member` and `Room Admin` (if the client is the admin of the room).                                                               |
| `Finished Game Early`   | Waiting place                  | The player has finished answering all questions, though some players did not. Leaving behaves the same as in the `Game` state.                                                                                                                  |

---

### 🔁 Transitions

| From                  | To                    | Trigger/Event                  | Description                                       |
|-----------------------|-----------------------|--------------------------------|---------------------------------------------------|
|                       | `Login`               | Starting the client            | The client was connected.                         |
| `Login`               | `Menu`                | Login / Signup                 | The client has been authenticated.                |
| `Menu`                | `Login`               | Logout                         | User logged out.                                  |
| `Menu`                | `Room Member`         | Join room                      | User joined a room.                               |
| `Menu`                | `Room Admin`          | Create room                    | User created a room.                              |
| `Room Member`         | `Menu`                | Leaving / Kicked / Admin left  | User going back to the menu.                      |
| `Room Admin`          | `Menu`                | Closing room                   | User going back to the menu and closing the room. |
| `Room Member/Admin`   | `Game`                | Admin started the game         | Game was started.                                 |
| `Game`                | `Finished Game Early` | User finished the current game | User finished the game.                           |
| `Finished Game Early` | `Room Member/Admin`   | Game finished                  | End of game.                                      |

---

# etc.