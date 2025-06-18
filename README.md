# TROME

<img src="./public/assets/banner.png" alt="Banner" width="600"/>

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

| From         | To            | Trigger/Event            | Description |
|--------------|---------------|---------------------------|-------------|
| `IDLE`       | `LOADING`     | Start Game                | Host starts the game. |
| `LOADING`    | `ASKING`      | Questions Loaded          | Questions are ready. |
| `ASKING`     | `ANSWERING`   | Question Displayed        | Timer starts for answers. |
| `ANSWERING`  | `EVALUATING`  | Timer Ends / All Answered | Time’s up or all answers in. |
| `EVALUATING` | `SCOREBOARD`  | Evaluation Complete       | Scores are updated. |
| `SCOREBOARD` | `ASKING`      | Next Question             | Move to next question. |
| `SCOREBOARD` | `FINISHED`    | No More Questions         | End of game. |

---

# etc.