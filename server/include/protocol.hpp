
#include <iostream>
#include <map>
#include <SmartBuffer.hpp>
#include "client.hpp"

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
            REMOVE_PLAYER,
        };

        Protocol();
        ~Protocol();
        static Protocol& get();
        void create_player(int id, std::map<int, Client>& clients, SmartBuffer& smartBuffer);
        void create_player_callback(int id, std::map<int, Client>& clients, SmartBuffer& smartBuffer);
        void create_player_broadcast(int id, std::map<int, Client>& clients, SmartBuffer &smartBuffer);
        void createMap(int id, std::map<int, Client>& clients, SmartBuffer& smartBuffer);
        void update_position(int id, std::map<int, Client>& clients, SmartBuffer& smartBuffer);
        void handle_message(int id, int clientSocket, std::map<int, Client>& clients, SmartBuffer& smartBuffer);
        void check_food_collision(int clientId, const std::pair<float, float>& clientPos, Client& client, SmartBuffer& smartBuffer);
        void check_player_collision(std::map<int, Client>& clients);

    private:
        std::string _buffer;
};