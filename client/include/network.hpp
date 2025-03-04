/*
** EPITECH PROJECT, 2024
** Agario
** File description:
** network
*/

#pragma once

#include <netinet/in.h>
#include <iostream>
#include <SmartBuffer.hpp>
#include "Entity.hpp"

class Network {
    public:
        Network() : _port(8080), _ip("127.0.0.1") {}
        Network(std::string& ip, int port);
        ~Network();
        void connectToServer(std::string &name);
        void receiveData(SmartBuffer &smartBuffer);
        void sendData(SmartBuffer &smartBuffer);
        void handleMessages(std::pair<float, float> direction);
        int getSocket() const;
        std::map<int, GameEngine::Entity> getEntities() const;
        bool isDead() const;

    private:
        std::string _ip;
        int _port;
        int _socket;
        bool _isDead = false;
};