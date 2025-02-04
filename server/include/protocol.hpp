
#include <iostream>
#include <map>
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
        void create_player(std::map<int, Client>& clients, std::string name);
        void create_player_callback(std::map<int, Client>& clients);
        void create_player_broadcast(std::map<int, Client>& clients);
        void update_position(int id, std::map<int, Client>& clients, std::pair<float, float> direction);
        bool handle_message(int id, int clientSocket, std::string message);
        void check_food_collision(int clientId, const std::pair<float, float>& clientPos, Client& client);
        void check_player_collision(std::map<int, Client>& clients);

    private:
        std::string _buffer;
};