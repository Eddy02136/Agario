/*
** EPITECH PROJECT, 2025
** Agario
** File description:
** menu
*/

#pragma once

#include <System.hpp>
#include <game.hpp>
#include <components/Button.hpp>
#include <components/ButtonRect.hpp>
#include <components/Color.hpp>
#include <components/Link.hpp>
#include <components/OptionButton.hpp>
#include <components/Position.hpp>
#include <components/Shape.hpp>
#include <components/Slider.hpp>
#include <components/Sprite.hpp>
#include <components/Text.hpp>
#include <components/Texture.hpp>

class Menu {

    public:
        Menu();
        ~Menu();

        static Menu& get();

        GameEngine::Entity createEntityRect(int id, const std::pair<int, int> size, const std::vector<std::pair<float, float>> position, sf::Color color, std::function<void()> callback);
        GameEngine::Entity createEntityText(int id, const std::string text, const std::vector<std::pair<float, float>> position, unsigned int fontSize);
        GameEngine::Entity createEntityInput(int id, int fontSize, const std::vector<std::pair<float, float>> position, std::string inputVar);
        void setupInput(const sf::Event& event);
        void displayMainMenu(sf::RenderWindow& window, GameEngine::System system);
        bool getIsPlayed();

    private:
        std::map<int, GameEngine::Entity> _entitiesMenu;
        bool _entitiesInitialized = false;
        bool _isPlayed = false;
        bool _isUsernameClicked = false;
        int _usernameId;
};
     
