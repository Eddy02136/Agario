# Entity

An entity is identified by an ID and contains all the components that represent its data.

## Functions

- `Entity(int id, Args&&... args)` -> Creates an entity with an ID and adds all the components provided in `args`.
- `void addComponent(const ComponentType& component)` -> Adds a component in an entity.
- `void removeComponent()` -> Removes component from an entity.
- `ComponentType& getComponent()` -> Retrieves a component from an entity.
- `bool hasComponent() const` -> Checks if an entity has a component.
- `int getEntityId() const` -> Retrieves the ID of an entity.

## Example

```cpp
#include <Entity.hpp>
#include <components/Position.hpp>
#include <components/Text.hpp>
#include <components/Color.hpp>
#include <iostream>

int main() {
    GameEngine::Entity entity(1, Text("Hello, World!", "Arial.ttf"), Position({{0, 0}, {1, 1}}));

    entity.addComponent(Color({255, 0, 0, 1}));
    entity.removeComponent<Color>();

    if (entity.hasComponent<Position>()) {
        std::cout << "Entity has a position component" << std::endl;
    }

    return 0;
}
```