# Agario Protocol Documentation

This document provides a detailed overview of the communication protocol for the Agario server. It outlines the purpose, payload, and transmission method (TCP) for each operation code (OpCode). The protocol ensures consistent communication between the server and clients.

---

## Overview

### Transport Protocols
- **TCP**: Provides reliable communication, ensuring that operations requiring acknowledgment are successfully delivered.

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
- **Description**: Default operation code.
- **Payload**: None.
- Sent To: Server, Clients.
- Transport: TCP.

### 2. **CREATE_PLAYER**
- **Value**: `1`
- **Description**: Creates a new player on the server.
- **Payload**:
  - `ID` (int16_t): Unique player ID.
  - `Name` (string): Player name.
- Sent To: Server.
- Transport: TCP.

### 3. **CREATE_PLAYER_CALLBACK**
- **Value**: `2`
- **Description**: Acknowledges the creation of a new player.
- **Payload**:
  - `ID` (int16_t): Unique player ID.
  - `Name` (string): Player name.
  - `Position` (int16_t, int16_t): Initial player position.
  - `Size` (int16_t): Initial player size.
  - `TextSize` (unsigned int): Initial player text size.
- Sent To: Clients.
- Transport: TCP.

### 4. **CREATE_PLAYER_BROADCAST**
- **Value**: `3`
- **Description**: Broadcasts the creation of a new player.
- **Payload**:
  - `ID` (int16_t): Unique player ID.
  - `Name` (string): Player name.
  - `Position` (int16_t, int16_t): Initial player position.
  - `Size` (int16_t): Initial player size.
  - `TextSize` (unsigned int): Initial player text size.
- Sent To: Clients.
- Transport: TCP.

### 5. **UPDATE_PLAYER**
- **Value**: `4`
- **Description**: Send the player’s mouse coordinates.
- **Payload**:
  - `ID` (int16_t): Unique player ID.
  - `Direction mouse` (float_t, float_t): Mouse direction.
- Sent To: Server.
- Transport: TCP.

### 6. **UPDATE_PLAYER**
- **Value**: `4`
- **Description**: Update the player’s position.
- **Payload**:
  - `ID` (int16_t): Unique player ID.
  - `Position` (int16_t, int16_t): Player position.
- Sent To: Client.
- Transport: TCP.

### 7. **CREATE_MAP**
- **Value**: `5`
- **Description**: Create the game map.
- **Payload**:
  - `ID` (int16_t): Unique map ID.
- Sent To: Client.
- Transport: TCP.

### 8. **ADD_FOOD**
- **Value**: `6`
- **Description**: Add food to the game map.
- **Payload**:
  - `ID` (int16_t): Unique map ID.
  - `Position` (int16_t, int16_t): Food position.
- Sent To: Client.
- Transport: TCP.

### 9. **REMOVE_FOOD**
- **Value**: `7`
- **Description**: Remove food from the game map.
- **Payload**:
  - `ID` (int16_t): Unique food ID.
  - `MAP_ID` (int16_t): Unique map ID.
  - `Position` (int16_t, int16_t): Food position.
  - `PLAYER_ID` (int16_t): Unique client ID.
  - `Size` (int16_t): Player size.
  - `TextSize` (unsigned int): Player text size.
  - `Score` (int16_t): Player score.`.
- Sent To: Client.
- Transport: TCP.

---

## Notes
- The server and clients must strictly adhere to the protocol to ensure consistency.
- Payloads must be interpreted exactly as defined.
