# HotkeyManager

The HotkeysManager class handles hotkey-to-keyboard mappings, key reassignment, and autofire state. It processes input events, triggers actions, and sends hotkey data over the network, using a singleton design.s

| Functions                          | Description                                                                                       |
|-------------------------------------|---------------------------------------------------------------------------------------------------|
| getAutoFireState                    | Returns the current state of the autofire feature (enabled or disabled).                         |
| checkKey                            | Checks if a triggered key event matches a registered hotkey and sends a network packet for the corresponding action. |
| isKeyUsed                           | Determines if a specific keyboard key is currently mapped to a hotkey.                           |
| isAutoFire                          | Toggles the autofire state when the Enter hotkey is pressed.                                      |
| keyToString                         | Converts a keyboard key to its corresponding string representation for display or debugging purposes. |