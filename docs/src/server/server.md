# Agario Server Architecture Documentation

The network game server is designed to manage multiple clients interacting in a game environment similar to **Agario**, where each client controls a player. The server handles connections, player movements, collisions between players and objects (such as food), and synchronization of information among all connected clients. The server is structured to work across multiple threads, handling data reception and sending efficiently.

### Main Components

1. **Server (Main Class)**
   - Manages server initialization, client connections, and data sending.
   - Uses a TCP socket to accept incoming client connections and utilizes the **select()** mechanism for file descriptor management.

2. **Client (Class representing each player)**
   - Manages information related to each client (socket, position, size, etc.).
   - Provides methods for updating and retrieving player information connected to the server.

3. **Protocol (Class responsible for managing communication protocols)**
   - Handles the serialization and deserialization of messages exchanged between the server and clients.
   - Processes game-specific operations (player creation, position update, collision handling, etc.).

4. **Map (Class representing the game map)**
   - Contains information about the game map, including positions of interactive objects (like food).
   - Manages adding and removing objects from the map, and provides methods to interact with those objects.

### Server Class Documentation

The Server class manages network communication for the server, including TCP connections with clients. It initializes and binds the socket, handles the addition of new clients, and uses fd_set to manage file descriptors with select() for monitoring incoming connections and messages.

## Functions

| Function                      | Description                                                                                                      |
|-------------------------------|------------------------------------------------------------------------------------------------------------------|
| **init**                       | Initializes the server by creating and binding the TCP socket, setting up the address, and starting the listener. |
| **manage_file_descriptors**    | Manages the fd_set for the TCP socket and client sockets, setting them up for monitoring with select().      |
| **sendToClient**               | Sends a message to a specific client using the SmartBuffer (handles size and data).                             |
| **sendToAllClients**           | Sends a message to all connected clients.                                                                         |
| **sendToAllClientsExcept**     | Sends a message to all clients except one, identified by their client ID.                                         |
| **add_client**                 | Accepts a new client connection, assigns it an ID, and creates a new thread to handle the client.                |
| **handle_client**              | Handles the communication with a specific client by receiving and processing messages in a separate thread.      |
| **run**                        | Main server loop that manages the file descriptors and accepts new client connections.                           |