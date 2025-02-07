# Systems

Systems are the core of the ECS architecture. They are the place where the logic of your game is implemented.

## Functions

- `void render(sf::RenderWindow& window, std::map<int, Entity>& entities)` -> Renders all entities that have a component capable of being rendered.
- `void update(int id, std::map<int, Entity>& entities, UpdateType type, const std::any& value, int posId = 0)` -> Update the entity with the given id. The type of update is specified by the `UpdateType` enum. The value is the new value of the component.

## Example

```cpp
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <map>
#include <components/Position.hpp>
#include <components/Link.hpp>
#include <components/Sprite.hpp>
#include <components/Text.hpp>
#include <components/Texture.hpp>
#include "Entity.hpp"
#include "System.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Client Game");
    std::map<int, GameEngine::Entity> entities;
    auto player = GameEngine::Entity(1, Sprite(), Texture("assets/sprite/spaceship.png", {0, 0, 34, 15}), Position({{0, 0}}));
    auto nametag = GameEngine::Entity(2, Text("Name", "assets/font/arial.ttf", 10), Position({{0, -14}}), Link(1));
    entities.emplace(1, std::move(player));
    entities.emplace(2, std::move(nametag));
    GameEngine::System system;
    float x, y = 0.0f;
    float deltaTime = 0.0f;
    float speed = 200.0f;
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        deltaTime = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            y -= speed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            y += speed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            x -= speed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            x += speed * deltaTime;
        }
        system.update(1, entities, GameEngine::UpdateType::Position, std::pair(x, y));
        window.clear();
        system.render(window, entities);
        window.display();
    }
    return 0;
}
```