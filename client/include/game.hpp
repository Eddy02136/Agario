//
// Created by Eddy Gardes on 27/01/2025.
//

#pragma once

class Game {
    public:

        static Game& get();

        void gameManager();
        std::pair<float, float> handlePlayerMovement(sf::RenderWindow& window, std::pair<float, float>& playerPosition);
        std::string getUsername();
        void setUsername(std::string username);
    private:
        Game();
        ~Game();
        float _speed = 5.0f;
        std::string _username = "";
        bool _isConnected = false;
};