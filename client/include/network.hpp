

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
        void handleSelect(std::pair<float, float> direction);
        void serialize ( const std::string &str, std::ostream &out, char key);
        void createPlayerCallback(SmartBuffer &SmartBuffer);
        void createPlayerBroadcast(SmartBuffer &SmartBuffer);
        void updatePosition(SmartBuffer &smartBuffer);
        void createMap(SmartBuffer &smartBuffer);
        void eatFood(SmartBuffer &smartBuffer);
        void addFood(SmartBuffer &smartBuffer);
        std::string deserialize(std::istream &in, char key);
        std::map<int, GameEngine::Entity> getEntities() const;

    private:
        std::map<int, GameEngine::Entity> _entities;
        std::string _ip;
        std::string _buffer;
        char _key = 0xA3;
        int _port;
        int _socket;
};