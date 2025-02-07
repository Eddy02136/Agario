# Protocol

The Protocol class handles the processing of various game-related messages, such as player creation, entity updates, health management, and more.The class includes methods for handling different types of network operations, including updating player positions, enemy data, bullets, and obstacles.

| Functions                          | Description                                                                                       |
|-------------------------------------|---------------------------------------------------------------------------------------------------|
| handleMessage                       | Handles incoming messages based on the opCode extracted from the buffer and calls the appropriate function for each message type. |
| handleCreatePlayerCallback          | Handles player creation by receiving the player’s ID and dimensions, then initializes their properties in the EntityManager. |
| handleCreatePlayerBroadcast         | Handles player creation via a broadcast message, initializing the player’s ID, name, and properties in the EntityManager. It allows retrieving information about the existence of other players. |
| handleUpdatePlayer                  | Updates a player’s position based on the received data and updates the EntityManager.             |
| handleUpdateViewport                | Updates the client’s viewport with the new viewport value.                                       |
| handleUpdateBlocks                  | Updates the obstacles based on the ID, coordinates, size, and type, then updates the EntityManager. |
| handleUpdateEnemies                 | Updates the position and properties of enemies based on their type and adds them to the EntityManager. |
| handleUpdatePlayerInfos             | Updates the player’s information such as score and kills based on the received data.             |
| handleUpdateBullets                 | Updates bullet information, including position and type, and adds them to the EntityManager.      |
| handleDeleteEntity                  | Deletes an entity from the EntityManager based on its ID, including related entities.            |
| handleUpdateEntityHealth            | Updates an entity’s health and manages win/loss conditions based on the entity’s health.         |