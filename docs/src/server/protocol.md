# R-Type Protocol Documentation

This document provides a detailed overview of the communication protocol for the R-Type server. It outlines the purpose, payload, and transmission method (TCP/UDP) for each operation code (OpCode). The protocol ensures consistent communication between the server and clients.

---

## Overview

### Transport Protocols
- **TCP**: Reliable communication for operations requiring acknowledgment (e.g., player creation, hotkey inputs).
- **UDP**: Lightweight and fast communication for real-time updates (e.g., player positions, bullets, obstacles).

### Message Structure
All messages follow this general structure:

| Field       | Type      | Size (bytes) | Description                               |
|-------------|-----------|--------------|-------------------------------------------|
| **OpCode**  | `int16_t` | 2            | Identifies the operation (see below).     |
| **Payload** | Varies    | Variable     | Data relevant to the specific operation.  |

---

## OpCode Definitions

### 1. **DEFAULT**
- **Value**: `0`
- **Description**: Used to save the client on the server.
- **Payload**: None.
- **Sent To**: Server or specific client.
- **Transport**: TCP.

### 2. **HOTKEY_PRESSED**
- **Value**: `1`
- **Description**: Notifies the server of a key press by a specific player.
- **Payload**:
  - `playerId` (int32_t): The ID of the player who pressed the key.
  - `hotkeyCode` (int16_t): The code of the pressed key.
- **Sent To**: Server.
- **Transport**: TCP.

### 3. **CREATE_PLAYER**
- **Value**: `10`
- **Description**: Requests the creation of a new player on the server.
- **Payload**:
  - `playerName` (string): The name of the player to create.
- **Sent To**: Server.
- **Transport**: TCP.

### 4. **CREATE_PLAYER_CALLBACK**
- **Value**: `11`
- **Description**: Confirms the creation of a new player to the requesting client.
- **Payload**:
  - `playerId` (int32_t): The ID of the newly created player.
  - `width` (int16_t): The width of the player’s sprite.
  - `height` (int16_t): The height of the player’s sprite.
- **Sent To**: Specific client (the one who requested the creation).
- **Transport**: TCP.

### 5. **CREATE_PLAYER_BROADCAST**
- **Value**: `12`
- **Description**: Broadcasts the creation of a new player to all connected clients.
- **Payload**:
  - `playerId` (int32_t): The ID of the new player.
  - `playerName` (string): The name of the new player.
  - `width` (int16_t): The width of the player’s sprite.
  - `height` (int16_t): The height of the player’s sprite.
- **Sent To**: All clients.
- **Transport**: UDP.

### 6. **UPDATE_PLAYERS**
- **Value**: `20`
- **Description**: Updates player positions and states.
- **Payload**:
  - `playerId` (int32_t): The ID of the player.
  - `posX` (int32_t): The player’s X-coordinate.
  - `posY` (int32_t): The player’s Y-coordinate.
- **Sent To**: All clients.
- **Transport**: UDP.

### 7. **UPDATE_VIEWPORT**
- **Value**: `21`
- **Description**: Updates the current map viewport.
- **Payload**:
  - `viewport` (double): The viewport’s position.
- **Sent To**: All clients.
- **Transport**: UDP.

### 8. **UPDATE_OBSTACLES**
- **Value**: `22`
- **Description**: Updates obstacle positions or states.
- **Payload**:
  - `obstacleId` (int32_t): The ID of the obstacle.
  - `posX` (int32_t): The obstacle’s X-coordinate.
  - `posY` (int32_t): The obstacle’s Y-coordinate.
  - `size` (int16_t): The size of the obstacle.
  - `type` (int16_t): The type of the obstacle.
- **Sent To**: All clients.
- **Transport**: UDP.

### 9. **UPDATE_BULLETS**
- **Value**: `23`
- **Description**: Updates bullet positions or states.
- **Payload**:
  - `bulletId` (int32_t): The ID of the bullet.
  - `posX` (int32_t): The bullet’s X-coordinate.
  - `posY` (int32_t): The bullet’s Y-coordinate.
  - `type` (int16_t): The type of the bullet.
- **Sent To**: All clients.
- **Transport**: UDP.

### 10. **UPDATE_ENEMIES**
- **Value**: `24`
- **Description**: Updates enemy positions or states.
- **Payload**:
  - `enemyId` (int32_t): The ID of the enemy.
  - `posX` (int32_t): The enemy’s X-coordinate.
  - `posY` (int32_t): The enemy’s Y-coordinate.
  - `width` (int16_t): The width of the enemy’s sprite.
  - `height` (int16_t): The height of the enemy’s sprite.
  - `type` (int16_t): The type of the enemy.
- **Sent To**: All clients.
- **Transport**: UDP.

### 11. **UPDATE_ENTITY_HEALTH**
- **Value**: `25`
- **Description**: Broadcasts changes in the health of an entity (player or enemy).
- **Payload**:
  - `entityId` (int32_t): The ID of the entity.
  - `health` (int16_t): The current health of the entity.
  - `maxHealth` (int16_t): The maximum health of the entity.
- **Sent To**: All clients.
- **Transport**: UDP.

### 12. **UPDATE_PLAYER_INFOS**
- **Value**: `26`
- **Description**: Updates player's infos.
- **Payload**:
  - `playerId` (int32_t): The ID of the player.
  - `kills` (int16_t): The kills amount of the player.
  - `score` (int32_t): The score of the player.
- **Sent To**: Specific client.
- **Transport**: UDP.

### 13. **DELETE_ENTITY**
- **Value**: `30`
- **Description**: Deletes an entity from the game (e.g., player, enemy, bullet).
- **Payload**:
  - `entityId` (int32_t): The ID of the entity to delete.
- **Sent To**: All clients.
- **Transport**: UDP.

---

## Notes
- The server and clients must strictly adhere to the protocol to ensure consistency.
- Payloads must be interpreted exactly as defined.
- TCP is used for operations requiring reliable delivery, while UDP is optimized for real-time updates.

