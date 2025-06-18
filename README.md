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
| `Game`                | `Finished Game Early` | User finished the current game  | User finished the game. |
| `Finished Game Early` | `Room Member/Admin`   | Game finished                   | End of game. |

---

## Codec model

### 📬 Message structure (Protocol)

#### 🧾 Request

| Field          | Size     | Description                  |
|----------------|----------|------------------------------|
| `Message Code` | 1 byte   | Message code identifier      |
| `Length`       | 4 bytes  | Length of the body in bytes (For the example: N bytes) |
| `Body`         | N bytes  | The data |


#### 🧾 Response

| Field          | Size     | Description                  |
|----------------|----------|------------------------------|
| `Message Type` | 1 byte   | Message type identifier (Response / Notification)     |
| `Message Code` | 1 byte   | Message code identifier      |
| `Length`       | 4 bytes  | Length of the body in bytes (For the example: N bytes) |
| `Body`         | N bytes  | The data |

#### 📦 Body

The body is determined by the message type. It may contain strings, IDs, answers, scores, or questions in JSON format. The Body is encrypted via the encryption (predetermined)

---

## 🔐 Cryptography Overview

This project uses modern cryptographic techniques to ensure secure communication between the **Server** and **Client**. Most of the encryption and decryption operations are powered by the [Crypto++](https://www.cryptopp.com/) library and .NET [System.Security.Cryptography](https://learn.microsoft.com/en-us/dotnet/api/system.security.cryptography?view=net-9.0).

NOTE: The keys are static, therefore it isn't fully secure. Don't use this as an example for an actual Cryptography example without making files at runtime. We did it that way because `Crypto++` and `System.Security.Cryptography` RSA keys don't fit for each other, we made the keys via openssl.
---

### 🔑 Current Encryption Method: RSA (Asymmetric)

- **Algorithm**: RSA (2048-bit keys)
- **Library**: `Crypto++` and  `System.Security.Cryptography`
- **Usage**: Used to encrypt session secrets and sensitive data.
- **Key Management**:
  - **Server** and **Client** each have their own RSA key pair.
  - Public/private keys are stored in `.pem` files.
  - Keys are loaded **at startup** from the files.

### 🛠 Other Encryption Method

#### 🟡 AES (Advanced Encryption Standard)

- **Algorithm**: AES-256 in CBC mode
- **Library**: `Crypto++` and  `System.Security.Cryptography`
- **Use Case**: Once a session is established using RSA, AES can encrypt bulk data with lower computational cost.

#### 🟡 OTP (One-Time Pad)

- **Use Case**: For lightweight or critical communications requiring unbreakable encryption (if keys are truly random and never reused).
- **Library**: None
- **Limitations**: Requires secure key exchange and perfect synchronization.

---

# etc.