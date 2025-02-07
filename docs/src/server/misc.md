# R-Type Miscellaneous Guide

This section covers various features of the project, such as adding enemies, obstacles, editing configurations, and understanding the logging system.

---

## Adding New Enemies

To add new enemies, you need to update both **EnemyManager** and **EnemyType enum** in `Enemy.hpp`.

### Steps to Add a New Enemy

1. **Define a New Enemy in the Enum**
   - Open `Enemy.hpp`.
   - Add a new entry to the `EnemyType` enum:
     ```cpp
     enum class EnemyType {
         NONE,
         GRUNT,
         SNIPER,
         TANK,
         SWARMER,
         BOSS,
         DRONE,
         MINION,
         CANNON,
         NEW_TYPE // Add your new enemy here
     };
     ```

2. **Configure the New Enemy**
   - Go to `EnemyManager.cpp`.
   - Add the new enemy type to `_enemyMapping`:
     ```cpp
     {"E009", {EnemyType::NEW_TYPE, 5, 60, 60, 800, 10, 1200, 700, 200}},
     ```
   - Here's a breakdown of the parameters:
     | **Parameter**   | **Description**                                                                 |
     |-----------------|---------------------------------------------------------------------------------|
     | `EnemyType`    | The type of enemy, linked to the enum.                                           |
     | `speed`        | Movement speed of the enemy.                                                     |
     | `width`        | Width of the enemy sprite (in pixels).                                           |
     | `height`       | Height of the enemy sprite (in pixels).                                          |
     | `bulletSpeed`  | Speed of the bullets fired by the enemy.                                         |
     | `bulletDamage` | Damage dealt by the enemy's bullets.                                             |
     | `shootCooldown`| Time between each shot (in milliseconds).                                        |
     | `shootRange`   | Range at which the enemy will start shooting (in pixels).                        |
     | `health`       | Total health points of the enemy.                                                |

3. **Map Integration**
   - Use the new enemy code (e.g., `E009`) in your map files to place the enemy.
   - Example map snippet:
     ```text
     B000B000B000E009B000B000
     ```

### Example for Adding "E009"
```cpp
{"E009", {EnemyType::DRONE, 4, 40, 40, 700, 8, 1000, 600, 100}},
```
This defines a new drone enemy:
- Moves at speed 4.
- Dimensions are 40x40 pixels.
- Bullet speed is 700 and damage is 8.
- Shoots every 1000 ms within a 600-pixel range.
- Health is 100.

---

## Adding New Obstacles

To add new obstacles:

1. **Update the ObstacleType Enum**
   - Open `ObstacleManager.hpp`.
   - Add a new type to `ObstacleType`:
     ```cpp
     enum class ObstacleType {
         NONE,
         OBSTACLE,
         OBSTACLE2,
         OBSTACLE3,
         NEW_OBSTACLE
     };
     ```

2. **Update the Obstacle Mapping**
   - Go to `ObstacleManager.cpp`.
   - Add the new obstacle type:
     ```cpp
     {"B005", ObstacleType::NEW_OBSTACLE},
     ```

3. **Map Integration**
   - Use the new obstacle code (e.g., `B005`) in your map files.
   - Example:
     ```text
     B000B005B000B000B000
     ```

---

## Configuration (Config.hpp)

The `Config.hpp` file defines constants for gameplay, server settings, and default values. Here's a table of key configurations:

| **Category**  | **Constant**           | **Description**                                              |
|---------------|------------------------|--------------------------------------------------------------|
| **Socket**    | `DEFAULT_BYTES`        | Default buffer size for socket communication.               |
|               | `PORT`                 | Port number for the server.                                  |
| **Server**    | `CADENCY`              | Frame rate interval in ms (affects `TICK_PER_SECOND`).       |
|               | `TICK_PER_SECOND`      | Server update frequency.                                     |
| **Program**   | `SUCCESS`              | Exit code for successful operations.                         |
|               | `ERROR`                | Exit code for errors.                                        |
| **Player**    | `PLAYER_WIDTH`         | Width of player sprites.                                     |
|               | `PLAYER_SPEED`         | Movement speed of players.                                   |
| **Map**       | `MAP_SPEED`            | Speed at which the map scrolls.                              |
|               | `MAP_WIDTH`/`HEIGHT`  | Dimensions of the map.                                       |
| **Obstacle**  | `OBSTACLE_SIZE`        | Size of obstacle sprites.                                    |

---

## Logging System

The `Logger` provides different log levels for debugging and monitoring.

### Log Levels and Colors

| **Log Level** | **Color** | **Usage**                          |
|---------------|-----------|------------------------------------|
| `info`        | Blue      | General information.               |
| `success`     | Green     | Successful operations.             |
| `warning`     | Yellow    | Warnings that don't halt execution.|
| `error`       | Red       | Critical errors.                   |
| `socket`      | Cyan      | Socket-related logs.               |
| `packet`      | Magenta   | Packet-specific logs.              |
| `thread`      | Pink      | Thread operations.                 |
| `debug`       | White     | Debugging information.             |
| `trace`       | Gray      | Detailed execution traces.         |

### How to Log Messages

Use the static methods from `Logger`:

```cpp
Logger::info("Server started.");
Logger::error("Failed to bind socket.");
Logger::debug("Player initialized.");
```

Logs are printed with timestamps for easy tracking.
