

#pragma once

#include <netinet/in.h>
#include <iostream>
#include "Entity.hpp"

class Network {
    public:
        Network();
        ~Network();
        void connectToServer();
        std::string receiveData();
        void handleSelect();
        std::map<int, GameEngine::Entity> getEntities() const;

    private:
        std::map<int, GameEngine::Entity> _entities;
        std::string _ip;
        int _port;
        int _socket;
};