//
// Created by Eddy Gardes on 27/01/2025.
//

#pragma once
#include <thread>
#include "network.hpp"

class Game {
    public:

        static Game& get();

        void gameManager();
        std::string getUsername();
        void setUsername(std::string username);
        void networkThread(Network &network);
    private:
        Game();
        ~Game();
        float _speed = 5.0f;
        std::string _username = "";
        std::thread _networkThread;
        std::pair<float, float> _direction;
        bool _isConnected = false;
};