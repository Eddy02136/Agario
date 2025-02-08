

#pragma once

#include <netinet/in.h>
#include <iostream>
#include <SmartBuffer.hpp>
#include "Entity.hpp"

class Network {
    public:
        Network();
        ~Network();
        void connectToServer(std::string &name);
        void receiveData(SmartBuffer &smartBuffer);
        void sendData(SmartBuffer &smartBuffer);
        void handleMessages(std::pair<float, float> direction);
        int getSocket() const;
        std::map<int, GameEngine::Entity> getEntities() const;

    private:
        std::string _ip;
        int _port;
        int _socket;
};