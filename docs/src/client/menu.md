# Menu

The Menu class manages the user interface for the game’s menu system, including buttons, input fields, and sprites. It handles the creation and rendering of menu entities, input handling, and transitions between different menu states.

| Functions                          | Description                                                                                       |
|-------------------------------------|---------------------------------------------------------------------------------------------------|
| createEntityButton                  | Creates a button entity with a title, font, size, position, and a callback function to be triggered on click. |
| createEntitySprite                  | Creates a sprite entity with given size, texture, texture rectangle, and position.                 |
| createEntityRect                    | Creates a rectangular button entity with size, position, color, and a callback function to be triggered on click. |
| createEntityInput                   | Creates an input field entity with a font, size, position, and default text value for user input.  |
| isClickedInput                      | Updates the state of whether the input fields (IP, port, or username) have been clicked.         |
| setupInput                          | Handles text input events to update the IP, port, or username fields based on user input.         |
| initMainMenu                        | Initializes and displays the main menu, creating and positioning all necessary UI elements.       |
| displayMenu                         | Displays the appropriate menu (Main or Options) based on the current menu state, calling the relevant display functions. |
