

#pragma once

#include <map>
#include <SmartBuffer.hpp>
#include <Entity.hpp>

class Protocol {
    public:
        enum OpCode {
            DEFAULT,
            CREATE_PLAYER,
            CREATE_PLAYER_CALLBACK,
            CREATE_PLAYER_BROADCAST,
            UPDATE_POSITION,
            CREATE_MAP,
            ADD_FOOD,
            REMOVE_FOOD,
            EAT_PLAYER,
            REMOVE_PLAYER,
            DEAD_PLAYER,
        };

        static Protocol& get();
        std::map<int, GameEngine::Entity> getEntities() const;
        void createPlayerCallback(SmartBuffer &SmartBuffer);
        void createPlayerBroadcast(SmartBuffer &SmartBuffer);
        void updatePosition(SmartBuffer &smartBuffer);
        void createMap(SmartBuffer &smartBuffer);
        void addFood(SmartBuffer &smartBuffer);
        void eatFood(SmartBuffer &smartBuffer);
        void eatPlayer(SmartBuffer &smartBuffer);
        void removePlayer(SmartBuffer &smartBuffer);

    private:
        Protocol();
        ~Protocol();
        std::map<int, GameEngine::Entity> _entities;
};