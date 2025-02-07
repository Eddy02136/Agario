# Client

The Client class manages the overall operation of the client.

It orchestrates the various components required for the game’s operation, from initializing the server to managing the entities to be displayed.

| Function                              | Description                                                                                       |
|-------------------------------------|---------------------------------------------------------------------------------------------------|
| runNetworkClient                    | This function starts the server.                                                                  |
| initializeNetwork                   | This function handles the server initialization.                                                  |
| manageBackground                    | This function simulates the game background being in motion. In reality, it replicates the given image and scrolls it. |
| manageSound                         | This function setup the sound of the game.                                                        |
| processEvents                       | This is the function containing the loop that handles all game events (mouse clicks, keyboard key presses, etc.). |
| handleAutoFire                      | This function handles the auto-fire event when the corresponding key is pressed.                 |
| initializeServer                    | The initializeServer function establishes the network connection with the server (TCP/UDP) and initializes the required resources. |
| updateGameState                     | This function calls the GameEngine and requests it to display all the entities contained in a list. |
| manageClient                        | The manageClient function is the main loop of the client. It initializes the game window, the necessary components (sound, network, menus, etc.), and manages the game lifecycle, including rendering, user events, state management (menu, gameplay, victory, defeat), and communication with the server. It runs as long as the window remains open. |
