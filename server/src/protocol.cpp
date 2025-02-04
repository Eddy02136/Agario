#include <iostream>
#include "config.hpp"
#include "protocol.hpp"
#include <string>
#include <sstream>
#include <vector>
#include "map.hpp"
#include "server.hpp"

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
    if (clients.empty()) return;

    auto lastClient = --clients.end();
    std::string data = std::to_string(OpCode::CREATE_PLAYER_CALLBACK) + " " +
                       std::to_string(lastClient->first) + " " +
                       std::to_string(lastClient->second.getPosition().first) + " " +
                       std::to_string(lastClient->second.getPosition().second) + "\n";
    for (auto &client : clients) {
        if (client.first != lastClient->first) {
            data += std::to_string(OpCode::CREATE_PLAYER_BROADCAST) + " " +
                               std::to_string(client.first) + " " +
                               std::to_string(client.second.getPosition().first) + " " +
                               std::to_string(client.second.getPosition().second) + "\n";
        }
    }
    data += std::to_string(OpCode::CREATE_MAP) + " " + std::to_string(Map::get().getId()) + "\n";
    std::map<int, std::pair<int, int>> map = Map::get().getMap();
    for (const auto &food : map) {
        data += std::to_string(OpCode::ADD_FOOD) + " " + std::to_string(Map::get().getId()) + " " + std::to_string(food.second.first) + " " + std::to_string(food.second.second) + "\n";
    }
    std::lock_guard<std::mutex> lock(Server::get().getQueueMutex());
    Server::get().getQueue().push({lastClient->second.getSocket(), Packet::dest::ONE, data});
    Server::get().getCond().notify_one();
}

void Protocol::create_player_broadcast(std::map<int, Client>& clients) {
    if (clients.empty()) return;

    auto lastClient = --clients.end();
    std::pair<float, float> pos = lastClient->second.getPosition();
    std::string newPlayerData = std::to_string(OpCode::CREATE_PLAYER_BROADCAST) + " " +
                                std::to_string(lastClient->first) + " " +
                                std::to_string(pos.first) + " " +
                                std::to_string(pos.second) + "\n";
    std::lock_guard<std::mutex> lock(Server::get().getQueueMutex());
    Server::get().getQueue().push({lastClient->second.getSocket(), Packet::dest::OTHERS, newPlayerData});
    Server::get().getCond().notify_one();
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
    if (clientPos.first < 0 || clientPos.first > Map::get().getWidth() || clientPos.second < 0 || clientPos.second > Map::get().getHeight()) {
        return;
    }
    client->second.setPosition(clientPos);
    for (auto &client : clients) {
        std::string data = std::to_string(OpCode::UPDATE_POSITION) + " " + std::to_string(id) + " " + std::to_string(clientPos.first) + " " + std::to_string(clientPos.second) + "\n";
        std::lock_guard<std::mutex> lock(Server::get().getQueueMutex());
        Server::get().getQueue().push({client.second.getSocket(), Packet::dest::ONE, data});
        Server::get().getCond().notify_one();
    }
}

bool Protocol::handle_message(int id, int clientSocket, std::string message) {
    try {
        std::map<int, Client>& clients = Server::get().getClients();
        if (message.empty()) {
            return true;
        }

        int opCode = 0;
        float x = 0;
        float y = 0;
        std::string name;
        
        std::vector<std::string> datas = splitString(message, '\n');
        for (const auto &line : datas) {
            if (line.empty()) continue;

            std::vector<std::string> args = splitString(line, ' ');
            opCode = std::stoi(args[0]);

            switch (opCode) {
                case CREATE_PLAYER:
                    if (args.size() == 2) {
                        name = args[1];
                        Protocol::get().create_player(clients, name);
                    }
                    break;

                case UPDATE_POSITION:
                    if (args.size() == 3) {
                        x = std::stof(args[1]);
                        y = std::stof(args[2]);
                        Protocol::get().update_position(id, clients, {x, y});
                    }
                    break;

                default:
                    std::cerr << "[Server] Unknown operation code: " << opCode << std::endl;
                    break;
            }
        }
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[Server] Exception in client thread: " << e.what() << std::endl;
        return true;
    }
}

Protocol::~Protocol() {}