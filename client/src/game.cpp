
#include <SFML/Graphics.hpp>
#include <cmath>
#include "network.hpp"
#include "System.hpp"
#include "game.hpp"
#include "menu.hpp"

Game::Game() {}

static std::pair<float, float> normalize(const std::pair<float, float>& vector) {
    float length = std::sqrt(vector.first * vector.first + vector.second * vector.second);
    if (length != 0)
        return std::pair<float, float>(vector.first / length, vector.second / length);
    return std::pair<float, float>(0, 0);
}

void Game::gameManager() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Agario");
    Network network;
    Menu menu;
    GameEngine::System system;
    network.connectToServer();

    std::pair<float, float> playerPosition(640.0f, 360.0f);
    std::pair<float, float> direction(0.0f, 0.0f);

    sf::View view(sf::FloatRect(0, 0, 1280, 720));
    window.setView(view);

    while (window.isOpen()) {
        //network.handleSelect(direction);
        window.clear();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (!Menu::get().getIsPlayed()) {
            Menu::get().displayMainMenu(window, system);
        } else {
            network.handleSelect(direction);
        }

        view.setCenter(playerPosition.first, playerPosition.second);
        window.setView(view);

        direction = handlePlayerMovement(window, playerPosition);

        std::map<int, GameEngine::Entity> entities = network.getEntities();
        system.render(window, entities);
        window.display();
    }
}

std::pair<float, float> Game::handlePlayerMovement(sf::RenderWindow& window, std::pair<float, float>& playerPosition) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    std::pair<float, float> targetPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

    std::pair<float, float> direction = { targetPosition.first - playerPosition.first, targetPosition.second - playerPosition.second };
    std::pair<float, float> normalizedDirection = normalize(direction);
    return normalizedDirection;
}

Game::~Game() {}