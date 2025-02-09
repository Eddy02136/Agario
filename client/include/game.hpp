/*
** EPITECH PROJECT, 2024
** Agario
** File description:
** game
*/

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
        void setIp(const std::string& ip);
        void setPort(int port);
        std::string& getIp();
        int getPort();
    private:
        Game();
        ~Game();
        float _speed = 5.0f;
        std::string _username = "";
        std::string _ip = "";
        int _port = 0;
        std::thread _networkThread;
        std::pair<float, float> _direction;
        bool _isConnected = false;
};