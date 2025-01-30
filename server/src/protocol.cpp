#include <iostream>
#include "config.hpp"
#include "protocol.hpp"
#include <string>
#include <sstream>
#include <vector>
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
    Server::get().sendToClient(lastClient->second.getSocket(), data);
}

void Protocol::create_player_broadcast(std::map<int, Client>& clients) {
    if (clients.empty()) return;

    auto lastClient = --clients.end();
    std::pair<float, float> pos = lastClient->second.getPosition();
    std::string newPlayerData = std::to_string(OpCode::CREATE_PLAYER_BROADCAST) + " " +
                                std::to_string(lastClient->first) + " " +
                                std::to_string(pos.first) + " " +
                                std::to_string(pos.second) + "\n";
    Server::get().sendToAllClientsExcept(lastClient->first, newPlayerData);
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
    for (auto &client : clients) {
        std::string data = std::to_string(OpCode::UPDATE_POSITION) + " " + std::to_string(id) + " " + std::to_string(clientPos.first) + " " + std::to_string(clientPos.second) + "\n";
        Server::get().sendToClient(client.second.getSocket(), data);
    }
}

bool Protocol::handle_message(int id, int clientSocket, std::map<int, Client>& clients) {
    try {
        std::string data = Server::get().receiveFromClient(clientSocket);
        if (data.empty()) {
            return true;
        }

        int opCode = 0;
        float x = 0;
        float y = 0;
        std::string name;
        
        std::vector<std::string> datas = splitString(data, '\n');
        if (_buffer.length() > 0) {
            datas[0] = _buffer + datas[0];
            _buffer.clear();
        }
        if (data[data.length() - 1] != '\n') {
            _buffer = datas[datas.size() - 1];
            datas.pop_back();
        }
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