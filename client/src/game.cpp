
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include "System.hpp"
#include "game.hpp"
#include "menu.hpp"

Game::Game() {}

Game& Game::get() {
    static Game instance;
    return instance;
}

static std::pair<float, float> normalize(const std::pair<float, float>& vector) {
    float length = std::sqrt(vector.first * vector.first + vector.second * vector.second);
    if (length != 0)
        return std::pair<float, float>(vector.first / length, vector.second / length);
    return std::pair<float, float>(0, 0);
}

std::atomic<bool> _isConnected{false};

void Game::networkThread(Network &network)
{
    try {
        while (_isConnected) {
            network.handleSelect(_direction);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Game::gameManager() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Agario");
    Network network;
    Menu menu;
    GameEngine::System system;
    std::pair<float, float> playerPosition(640.0f, 360.0f);
    std::pair<float, float> direction(0.0f, 0.0f);
    //window.setFramerateLimit(60);

    while (window.isOpen()) {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                _isConnected = false;
            }
            Menu::get().setupInput(event);
        }
        
        if (!Menu::get().getIsPlayed()) {
            Menu::get().displayMainMenu(window, system);
        } else {
            if (!_isConnected) {
                network.connectToServer(_username);
                _isConnected = true;
                if (_networkThread.joinable()) {
                    _networkThread.join();
                }
                _networkThread = std::thread(&Game::networkThread, this, std::ref(network));
            }
            std::map<int, GameEngine::Entity> entities = network.getEntities();
            _direction = handlePlayerMovement(window, playerPosition);
            system.render(window, entities);
        }
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

std::string Game::getUsername() {
    return this->_username;
}

void Game::setUsername(std::string username) {
    _username = username;
}

Game::~Game() {}