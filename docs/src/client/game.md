# Game

The `Game` class manages the overall operation of the client.

It orchestrates the various components required for the game’s operation, from initializing the server to managing the entities to be displayed.

| Function               | Description                                                                                      |
|------------------------|--------------------------------------------------------------------------------------------------|
| `networkThread`         | This function handles the network thread, responsible for communication with the server.         |
| `gameManager`           | This function manages the game window, including display and user input handling.                |