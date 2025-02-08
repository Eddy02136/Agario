# Protocol

The Protocol class handles the processing of various game-related messages, such as player creation, entity updates and more.The class includes methods for handling different types of network operations, including updating player positions, enemy data, and obstacles.

| Function                | Description |
|-------------------------|-------------|
| **createPlayerCallback**  | Handles player creation by receiving the player's ID, name, and dimensions, then initializes their properties. |
| **createPlayerBroadcast** | Handles player creation via a broadcast message, initializing the player's ID, name, and properties. It also provides information about the existence of other players. |
| **updatePosition**        | Updates a player's position based on the received data. |
| **createMap**            | Handles the creation of a map by receiving the map's ID. |
| **addFood**             | Adds food to the game based on the received data. |
| **eatFood**             | Removes food from the game based on the received data and updates the size of the player who ate it. |