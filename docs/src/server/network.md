# R-Type Network Architecture Documentation

This document provides a detailed explanation of the R-Type network implementation. It describes the structure, threading model, client management, and mechanisms for handling communication between the server and clients using TCP and UDP protocols.

---

## Overview

The R-Type network is designed to handle real-time multiplayer interactions with a balance between reliability and performance. The server uses a dual-protocol approach:

- **TCP**: Ensures reliable delivery of critical operations such as player creation and key press events.
- **UDP**: Optimized for real-time updates like player positions, bullet movements, and obstacle updates, where occasional packet loss is acceptable.

---

## Architecture and Structure

### Server Components
1. **Main Server Class (`Server`)**
   - Orchestrates the overall network flow.
   - Initializes both TCP and UDP sockets.
   - Starts separate threads for handling TCP and UDP operations.

2. **TCP Socket (`TcpSocket`)**
   - Manages reliable client-server communication.
   - Handles client connections, disconnections, and data reception.
   - Binds each client connection to a specific player.

3. **UDP Socket (`UdpSocket`)**
   - Manages real-time communication.
   - Broadcasts updates to all clients.
   - Operates three main loops:
     - **Read Loop**: Reads incoming UDP packets.
     - **Update Loop**: Processes game logic.
     - **Send Loop**: Sends updates to clients.

4. **SmartBuffer**
   - Handles serialization and deserialization of data.
   - Used to encode and decode protocol-compliant messages.

---

## Threading Model

The server uses a multi-threaded approach to handle TCP and UDP operations concurrently:

### Threads
1. **TCP Thread**:
   - Handles client connections and disconnections via the `TcpSocket`.
   - Reads and processes incoming data using a dedicated thread per client.

2. **UDP Threads**:
   - **Read Thread**: Continuously receives packets from clients and processes them.
   - **Update Thread**: Runs the game logic, updating player positions, bullets, and obstacles.
   - **Send Thread**: Sends game state updates to all clients.

### Thread Synchronization
- **Mutexes**: Used to protect shared resources such as the list of connected clients and game entities.
- **Locks**: Ensures that no two threads modify the same data simultaneously.

---

## Client Management

### Storing Clients
Clients are stored in separate lists for TCP and UDP communications:

1. **TCP Clients**:
   - Maintained as a vector of socket file descriptors.
   - Each client is associated with a player ID, stored in the `PlayerManager`.

2. **UDP Clients**:
   - Stored as a vector of `sockaddr_in` structures.
   - Each client’s address is added when a UDP packet is received.

### Mapping Players to Clients
- When a new player is created via TCP, their `playerId` is mapped to their TCP socket.
- The same `playerId` is used for UDP communications to identify the player.

---

## TCP Workflow

### Connection Handling
1. **Initialization**:
   - The TCP socket is created and bound to a port.
   - The socket listens for incoming connections.

2. **Accepting Connections**:
   - New connections are accepted in a loop.
   - Each accepted client socket is passed to a dedicated thread for communication.

### Data Handling
1. **Receiving Data**:
   - Each client thread reads data from its associated socket.
   - Data is passed to the `Protocol` class for decoding and handling.

2. **Sending Data**:
   - Responses are serialized using `SmartBuffer` and sent back to the client.
   - Broadcasts are handled by iterating over the list of clients.

---

## UDP Workflow

### Initialization
- The UDP socket is created and bound to a port.
- Unlike TCP, there is no need to establish connections.

### Read Loop
- Continuously reads incoming packets from any client.
- Decodes messages using `Protocol` and updates the game state.

### Update Loop
- Executes game logic, such as updating player positions, bullets, and obstacles.
- Uses mutexes to synchronize shared resources.

### Send Loop
- Serializes the current game state into protocol-compliant messages.
- Sends updates to all connected clients using their stored addresses.

---

## Key Classes and Responsibilities

### 1. `Server`
- Entry point of the application.
- Initializes `TcpSocket` and `UdpSocket`.
- Manages threads for TCP and UDP workflows.

### 2. `TcpSocket`
- Handles client connections and reliable communication.
- Maintains a list of connected clients.
- Maps client sockets to player IDs.

### 3. `UdpSocket`
- Handles real-time communication.
- Maintains a list of client addresses.
- Reads, updates, and sends game state in separate threads.

### 4. `Protocol`
- Decodes incoming messages and routes them to the appropriate handler.
- Encodes outgoing messages to ensure protocol compliance.

### 5. `SmartBuffer`
- Provides serialization and deserialization utilities.
- Ensures efficient handling of binary data.

---

## Communication Flow

1. **Player Creation**:
   - A client sends a `CREATE_PLAYER` request via TCP.
   - The server creates a player and sends back a `CREATE_PLAYER_CALLBACK` response.
   - The server broadcasts a `CREATE_PLAYER_BROADCAST` message via UDP.

2. **Game Updates**:
   - The server’s update thread processes the game state (e.g., player movements, bullets).
   - The send thread broadcasts updates via UDP to all clients.

3. **Hotkey Presses**:
   - A client sends a `HOTKEY_PRESSED` message via TCP.
   - The server updates the player’s state based on the hotkey.

---

## Summary

The R-Type network implementation leverages the strengths of both TCP and UDP protocols to deliver reliable and efficient communication. The architecture ensures scalability and real-time performance while maintaining a clear separation of responsibilities between components. Mutexes and threads ensure thread safety, enabling smooth operation in a multiplayer environment.

