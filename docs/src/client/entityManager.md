# Entity Manager

The EntityManager class manages the creation, deletion, and updating of entities.

| Functions                          | Description                                                                                       |
|-------------------------------------|---------------------------------------------------------------------------------------------------|
| CompareEntities                     | This function compares the entity IDs with the list of already existing entities. If the ID exists, the entity should be updated; if it does not exist, a new entity should be created. |
| CreateEntity                        | This function creates the entities through the game engine based on the requested parameters.      |
| setPlayerColor                      | It allows targeting the correct sprite color on an image (to apply setTextureRect() function) based on the requested entity. |
| setEnemy                            | It allows targeting the correct enemy sprite on an image (to apply setTextureRect() function) based on the requested entity. |
| manageBackground                    | The function that creates the background entity.                                                  |
| winGame                             | The function checks if the player has won by verifying if the boss’s health is 0 or not.          |
| loseGame                            | The function checks if the player has lost by verifying if his health is 0 or not.                |

