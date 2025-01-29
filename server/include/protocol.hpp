
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
        };

        Protocol();
        ~Protocol();
        static Protocol& get();
        void create_player(std::map<int, Client>& clients, std::string name);
        void create_player_callback(std::map<int, Client>& clients);
        void create_player_broadcast(std::map<int, Client>& clients);
        void handle_message(int clientSocket, std::map<int, Client>& clients);
};