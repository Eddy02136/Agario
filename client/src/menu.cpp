/*
** EPITECH PROJECT, 2025
** Agario
** File description:
** menu.cpp
*/

#include <menu.hpp>

Menu::Menu() {}

Menu::~Menu() {}

Menu& Menu::get() {
    static Menu instance;
    return instance;
}

GameEngine::Entity Menu::createEntityRect(int id, const std::pair<int, int> size, const std::vector<std::pair<float, float>> position, sf::Color color, std::function<void()> callback) {
    auto rectEntity = GameEngine::Entity(id);
    auto buttonRect = ButtonRect(size, color, false);
    buttonRect.setCallback(callback);
    rectEntity.addComponent(buttonRect);
    rectEntity.addComponent(Position(position));
    return rectEntity;
}

GameEngine::Entity Menu::createEntityText(int id, const std::string text, const std::vector<std::pair<float, float>> position, unsigned int fontSize) {
    auto newEntity = GameEngine::Entity(id);
    newEntity.addComponent(Text(text, "font/Inter_Bold.ttf", fontSize));
    newEntity.addComponent(Position(position));
    newEntity.addComponent(Color({255, 255, 255, 255}));
    return newEntity;
}


GameEngine::Entity Menu::createEntityInput(int id, int fontSize, const std::vector<std::pair<float, float>> position, std::string inputVar) {
    auto inputEntity = GameEngine::Entity(id);
    inputEntity.addComponent(Text(inputVar, "font/Inter_Bold.ttf", fontSize));
    inputEntity.addComponent(Position(position));
    inputEntity.addComponent(Color({255, 255, 255, 255}));
    return inputEntity;
}


void Menu::displayMainMenu(sf::RenderWindow& window, GameEngine::System system) {
    if (!_entitiesInitialized) {
        int entityId = 0;

        _entitiesMenu.emplace(entityId, createEntityRect(entityId++, {170, 70}, {{590, 290}}, sf::Color::Transparent, [this]() { this->_isPlayed = true; }));

        _entitiesMenu.emplace( entityId, createEntityText(entityId++, "PLAY", {{680, 330}}, 50));
        _entitiesMenu.emplace(entityId, createEntityRect(entityId++, {170, 70}, {{590, 390}}, sf::Color::Transparent, [this, &window]() { window.close(); }));

        _entitiesMenu.emplace( entityId, createEntityText(entityId++, "QUIT", {{680, 430}}, 50));
        _entitiesInitialized = true;
    }
    system.render(window, _entitiesMenu);
}


bool Menu::getIsPlayed() {
    return this->_isPlayed;
}