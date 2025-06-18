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
- User-defined qiestions
- Head-to-Head gamemode
    - Pre-game countdown
- Cryptography
    - OTP
    - Crypto++
        - RSA
        - AES

### Custom
- CMake-aligned project
- Cros-platform support for Windows/Unix
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

| State Name              | Purpose                           | Description |
|-------------------------|-----------------------------------|-------------|
| `Login`                 | Initial state / Authenticating    | The client started up and isn't logged as user |
| `Menu`                  | Menu                              | A point between the login and the game. Also a point for looking up some statistics and adding new questions |
| `Room Member`           | Waiting room                      | A place that a user can wait for the game to start while seeing the room data and the statistics of the users that are in the room. Leaving the room will notify everyone else |
| `Room Admin`            | Waiting room                      | Same as the `Room Member` State but can kick players, change the room data and statrt the game. Leaving the room will close it |
| `Game`                  | Playing the game                  | The actual game. Can request questions and submit answers. Leaving will do as same as `Room Member` or `Room Admin` (if the client is the admin of the room) |
| `Finished Game Early`   | Waiting place                     | The client has finished, but there are some players that didn't. Leaving will do the same as `Game` |

---

### 🔁 Transitions

| From                  | To                    | Trigger/Event                   | Description |
|-----------------------|-----------------------|---------------------------------|-------------|
|                       | `Login`               | Starting the client             | Client start up. |
| `Login`               | `Menu`                | Login / Signup                  | The client has been authenticated. |
| `Menu`                | `Login`               | Logout                          | User logged out. |
| `Menu`                | `Room Member`         | Join room                       | User joined an existing room. |
| `Menu`                | `Room Admin`          | Create room                     | User created a room. |
| `Room Member`         | `Menu`                | Leaving / Kicked / Admin left   | User going back to the menu. |
| `Room Admin`          | `Menu`                | Closing room                    | User going back to the menu and closing the room. |
| `Room Member/Admin`   | `Game`                | Admin started a game            | Starts a game. |
| `Game`                | `Finished Game Early` | User finished the current game  | User finnished the game. |
| `Finished Game Early` | `Room Member/Admin`   | Game finished                   | End of game. |

---

# etc.