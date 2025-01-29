//
// Created by Eddy Gardes on 27/01/2025.
//

#pragma once

class Game {
    public:
        Game();
        ~Game();
        void gameManager();
        std::pair<float, float> handlePlayerMovement(sf::RenderWindow& window, std::pair<float, float>& playerPosition);
    private:
        float _speed = 5.0f; 
};