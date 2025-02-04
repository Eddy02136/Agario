#include <iostream>
#include "config.hpp"
#include "protocol.hpp"
#include <string>
#include <sstream>
#include <vector>
#include "map.hpp"
#include "server.hpp"
#include <complex>

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
                       lastClient->second.getName() + " " +
                       std::to_string(lastClient->second.getPosition().first) + " " +
                       std::to_string(lastClient->second.getPosition().second) + " " +
                       std::to_string(lastClient->second.getSize()) + "\n";

    for (auto &client : clients) {
        if (client.first != lastClient->first) {
            data += std::to_string(OpCode::CREATE_PLAYER_BROADCAST) + " " +
                               std::to_string(client.first) + " " +
                               client.second.getName() + " " +
                               std::to_string(client.second.getPosition().first) + " " +
                               std::to_string(client.second.getPosition().second) + " " +
                               std::to_string(client.second.getSize()) + "\n";
        }
    }
    data += std::to_string(OpCode::CREATE_MAP) + " " + std::to_string(Map::get().getId()) + "\n";
    std::map<int, std::pair<int, int>> map = Map::get().getMap();
    for (const auto &food : map) {
        data += std::to_string(OpCode::ADD_FOOD) + " " + std::to_string(Map::get().getId()) + " " + std::to_string(food.second.first) + " " + std::to_string(food.second.second) + "\n";
    }
    Server::get().sendToClient(lastClient->second.getSocket(), data);
}

void Protocol::create_player_broadcast(std::map<int, Client>& clients) {
    if (clients.empty()) return;

    auto lastClient = --clients.end();
    std::pair<float, float> pos = lastClient->second.getPosition();
    int size = lastClient->second.getSize();
    std::string newPlayerData = std::to_string(OpCode::CREATE_PLAYER_BROADCAST) + " " +
                                std::to_string(lastClient->first) + " " +
                                lastClient->second.getName() + " " +
                                std::to_string(pos.first) + " " +
                                std::to_string(pos.second) + " " +
                                std::to_string(size) + "\n";
    Server::get().sendToAllClientsExcept(lastClient->first, newPlayerData);
}

void Protocol::check_food_collision(int clientId, const std::pair<float, float>& clientPos, Client& client) {
    auto foodMap = Map::get().getMap();

    float clientRadius = client.getSize(); 
    for (auto& food : foodMap) {
        int foodId = food.first;
        const auto& foodPos = food.second;


        float dx = (clientPos.first + clientRadius) - foodPos.first;
        float dy = (clientPos.second + clientRadius) - foodPos.second;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= (clientRadius)) {
            client.setSize(client.getSize() + 1);
            std::string data = std::to_string(OpCode::REMOVE_FOOD) + " " +
                               std::to_string(foodId) + " " +
                               std::to_string(Map::get().getId()) + " " +
                               std::to_string(foodPos.first) + " " +
                               std::to_string(foodPos.second) + " " +
                               std::to_string(clientId) + " " +
                               std::to_string(client.getSize()) + "\n";
            Map::get().removeFood(foodId);
            Server::get().sendToAllClients(data);
        }
    }
}

void Protocol::check_player_collision(std::map<int, Client>& clients) {
    for (auto it1 = clients.begin(); it1 != clients.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != clients.end(); ) {
            int id1 = it1->first;
            int id2 = it2->first;

            auto& player1 = it1->second;
            auto& player2 = it2->second;

            float size1 = player1.getSize();
            float size2 = player2.getSize();

            float radius1 = size1 / 2.0f;
            float radius2 = size2 / 2.0f;

            std::pair<float, float> pos1 = player1.getPosition();
            std::pair<float, float> pos2 = player2.getPosition();

            float centerX1 = pos1.first + radius1;
            float centerY1 = pos1.second + radius1;
            float centerX2 = pos2.first + radius2;
            float centerY2 = pos2.second + radius2;

            float dx = centerX1 - centerX2;
            float dy = centerY1 - centerY2;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= (radius1 + radius2)) {
                if (size1 > size2) {
                    player1.setSize(size1 + size2 / 2);
                    std::cout << "Player " << id1 << " ate Player " << id2 << std::endl;

                    std::string removeData = std::to_string(OpCode::REMOVE_PLAYER) + " " +
                                             std::to_string(id2) + " " +
                                             std::to_string(pos2.first) + " " +
                                             std::to_string(pos2.second) + "\n";

                    Server::get().sendToAllClients(removeData);
                    it2 = clients.erase(it2);
                } else if (size2 > size1) {
                    player2.setSize(size2 + size1 / 2);
                    std::cout << "Player " << id2 << " ate Player " << id1 << std::endl;

                    std::string removeData = std::to_string(OpCode::REMOVE_PLAYER) + " " +
                                             std::to_string(id1) + " " +
                                             std::to_string(pos1.first) + " " +
                                             std::to_string(pos1.second) + "\n";                    
                    Server::get().sendToAllClients(removeData);
                    it1 = clients.erase(it1);
                    break;
                } else {
                    ++it2;
                }
            } else {
                ++it2;
            }
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
    if (clientPos.first < 0 || clientPos.first > Map::get().getWidth() || clientPos.second < 0 || clientPos.second > Map::get().getHeight()) {
        return;
    }

    check_food_collision(id, clientPos, client->second);
    check_player_collision(clients);

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