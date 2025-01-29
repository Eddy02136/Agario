
#include <string>
#include <sstream>
#include "server.hpp"
#include "protocol.hpp"
#include <vector>


Protocol::Protocol() {}

Protocol& Protocol::get() {
    static Protocol instance;
    return instance;
}

static std::vector<std::string> splitString(const std::string &data, const char delimiter) {
    std::stringstream str(data);
    std::string line;
    std::vector<std::string> result;
    while (std::getline(str, line, delimiter)) {
        result.push_back(line);
    }
    return result;
}

void Protocol::create_player(std::map<int, Client>& clients, std::string name) {
    if (clients.empty()) {
        std::cerr << "Client list is empty!" << std::endl;
        return;
    }
    auto& lastClient = --clients.end();
    lastClient->second.setName(name);
    create_player_callback(clients);
    create_player_broadcast(clients);
}

void Protocol::create_player_callback(std::map<int, Client>& clients) {
    auto lastClient = --clients.end();
    std::string data = std::to_string(OpCode::CREATE_PLAYER_CALLBACK) + " " + std::to_string(lastClient->first) + " " + std::to_string(lastClient->second.getPosition().first) + " " + std::to_string(lastClient->second.getPosition().second) + "\n";
    for (const auto &client : clients) {
            if (client.first == lastClient->first) continue;
            std::pair<float, float> pos = client.second.getPosition();
            data += std::to_string(OpCode::CREATE_PLAYER_BROADCAST) + " " + std::to_string(client.first) + " " + std::to_string(pos.first) + " " + std::to_string(pos.second) + "\n";
        }
    Server::get().sendToClient(lastClient->second.getSocket(), data);
}

void Protocol::create_player_broadcast(std::map<int, Client>& clients) {
    std::string data;
    if (!clients.empty()) {
        auto lastClient = --clients.end();
        for (const auto &client : clients) {
            if (client.first == lastClient->first) continue;
            std::pair<float, float> pos = lastClient->second.getPosition();
            std::string newPlayerData = std::to_string(OpCode::CREATE_PLAYER_BROADCAST) + " " + std::to_string(lastClient->first) + " " + std::to_string(pos.first) + " " + std::to_string(pos.second) + "\n";
            Server::get().sendToClient(client.second.getSocket(), newPlayerData);
        }
    }
}

void Protocol::update_position(int id, std::map<int, Client>& clients, std::pair<float, float> direction) {
    auto client = clients.find(id);
    if (client == clients.end()) {
        std::cerr << "Client not found" << std::endl;
        return;
    }
    auto clientPos = client->second.getPosition();
    clientPos.first += direction.first * client->second.getSpeed();
    clientPos.second += direction.second * client->second.getSpeed();
    client->second.setPosition(clientPos);
    std::string data = std::to_string(OpCode::UPDATE_POSITION) + " " + std::to_string(id) + " " + std::to_string(clientPos.first) + " " + std::to_string(clientPos.second) + "\n";
    Server::get().sendToClient(client->second.getSocket(), data);
}

void Protocol::handle_message(int id, int clientSocket, std::map<int, Client>& clients) {
    try {
        std::string data = Server::get().receiveFromClient(clientSocket);
        std::vector<std::string> datas = splitString(data, '\n');
        int opCode = 0;
        float x = 0;
        float y = 0;
        std::string name;
        
        for (const auto &line : datas) {
            if (line.empty()) {
                continue;
            }
            std::vector<std::string> args = splitString(line, ' ');
            opCode = std::stoi(args[0]);
            switch (opCode) {
                case CREATE_PLAYER:
                    name = args[1];
                    Protocol::get().create_player(clients, name);
                    break;

                case UPDATE_POSITION:
                    x = std::stof(args[1]);
                    y = std::stof(args[2]);
                    Protocol::get().update_position(id, clients, {x, y});
                    break;

                case DEFAULT:
                    break;

                default:
                    break;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Failed to receive message" << e.what() << std::endl;
    }
}

Protocol::~Protocol() {}