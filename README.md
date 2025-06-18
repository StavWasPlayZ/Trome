# TROME

<img src="./public/assets/banner.png" alt="Banner" width="700"/>

Trome was created and submitted as part of the Magshimim National Cyber Program 2nd year final project.

Made by Itay Herskovits and Stav Solomon.

# Installation & Setup

# Server Infrastructure

## State management
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

# etc.