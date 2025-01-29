
#include <SFML/Graphics.hpp>
#include <cmath>
#include "network.hpp"
#include "System.hpp"
#include "game.hpp"

Game::Game() {}

void Game::gameManager() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Agario");
    Network network;
    GameEngine::System system;
    network.connectToServer();

    std::pair<float, float> playerPosition(640.0f, 360.0f);

    sf::View view(sf::FloatRect(0, 0, 1280, 720));
    window.setView(view);

    while (window.isOpen()) {
        network.handleSelect();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        view.setCenter(playerPosition.first, playerPosition.second);
        window.setView(view);

        handlePlayerMovement(window, playerPosition);

        window.clear();
        std::map<int, GameEngine::Entity> entities = network.getEntities();
        system.render(window, entities);
        window.display();
    }
}

std::pair<float, float> Game::handlePlayerMovement(sf::RenderWindow& window, std::pair<float, float>& playerPosition) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    std::pair<float, float> targetPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

    std::pair<float, float> direction = { targetPosition.first - playerPosition.first, targetPosition.second - playerPosition.second };
    
    return direction;

}

Game::~Game() {}