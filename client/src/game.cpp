
#include <SFML/Graphics.hpp>
#include "network.hpp"
#include "System.hpp"
#include "game.hpp"

Game::Game() {}

void Game::gameManager() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Agario");
    Network network;
    GameEngine::System system;
    network.connectToServer();

    while (window.isOpen()) {
        network.handleSelect();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        std::map<int, GameEngine::Entity> entities = network.getEntities();
        system.render(window, entities);
        window.display();
    }
}

Game::~Game() {}