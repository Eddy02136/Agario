/*
** EPITECH PROJECT, 2025
** Agario
** File description:
** protocol
*/

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

void Protocol::create_player(int id, std::map<int, Client>& clients, SmartBuffer& smartBuffer) {
    std::string name;
    smartBuffer >> name;
    auto client = clients.find(id);
    client->second.setName(name);
    create_player_callback(id, clients, smartBuffer);
    create_player_broadcast(id, clients, smartBuffer);
    createMap(id, clients, smartBuffer);
}

void Protocol::createMap(int id, std::map<int, Client>& clients, SmartBuffer& smartBuffer) {
    if (clients.empty()) return;
    auto client = clients.find(id);
    smartBuffer.reset();
    smartBuffer << static_cast<int16_t>(OpCode::CREATE_MAP) << static_cast<int16_t>(Map::get().getId());
    Server::get().sendToClient(client->second.getSocket(), smartBuffer);
    smartBuffer.reset();
    std::map<int, std::pair<int, int>> map = Map::get().getMap();
    for (const auto &food : map) {
        smartBuffer << static_cast<int16_t>(OpCode::ADD_FOOD)
                    << static_cast<int16_t>(Map::get().getId())
                    << static_cast<int16_t>(food.second.first)
                    << static_cast<int16_t>(food.second.second);
        Server::get().sendToClient(client->second.getSocket(), smartBuffer);
        smartBuffer.reset();
    }
}

void Protocol::create_player_callback(int id, std::map<int, Client>& clients, SmartBuffer &smartBuffer) {
    if (clients.empty()) return;

    auto client = clients.find(id);
    smartBuffer.reset();
    smartBuffer << static_cast<int16_t>(OpCode::CREATE_PLAYER_CALLBACK)
                << static_cast<int16_t>(id)
                << client->second.getName()
                << static_cast<int16_t>(client->second.getPosition().first)
                << static_cast<int16_t>(client->second.getPosition().second)
                << static_cast<int16_t>(client->second.getSize())
                << static_cast<unsigned int>(client->second.getTextSize());
    Server::get().sendToClient(client->second.getSocket(), smartBuffer);
}

void Protocol::create_player_broadcast(int id, std::map<int, Client>& clients, SmartBuffer &smartBuffer) {
    if (clients.empty()) return;
    auto client = clients.find(id);
    smartBuffer.reset();
    for (auto &tmpClient : clients) {
        if (tmpClient.first != client->first) {
            smartBuffer << static_cast<int16_t>(OpCode::CREATE_PLAYER_BROADCAST)
                        << static_cast<int16_t>(tmpClient.first)
                        << tmpClient.second.getName()
                        << static_cast<int16_t>(tmpClient.second.getPosition().first)
                        << static_cast<int16_t>(tmpClient.second.getPosition().second)
                        << static_cast<int16_t>(tmpClient.second.getSize())
                        << static_cast<unsigned int>(tmpClient.second.getTextSize());
            Server::get().sendToClient(client->second.getSocket(), smartBuffer);
            smartBuffer.reset();
        }
    }
    std::pair<float, float> pos = client->second.getPosition();
    int size = client->second.getSize();
    smartBuffer.reset();
    smartBuffer << static_cast<int16_t>(OpCode::CREATE_PLAYER_BROADCAST)
                << static_cast<int16_t>(id)
                << client->second.getName()
                << static_cast<int16_t>(pos.first)
                << static_cast<int16_t>(pos.second)
                << static_cast<int16_t>(size)
                << static_cast<unsigned int>(client->second.getTextSize());
    Server::get().sendToAllClientsExcept(id, smartBuffer);
}

void Protocol::check_food_collision(int clientId, const std::pair<float, float>& clientPos, Client& client, SmartBuffer &smartBuffer) {
    auto foodMap = Map::get().getMap();

    float clientRadius = client.getSize(); 
    for (auto& food : foodMap) {
        int foodId = food.first;
        const auto& foodPos = food.second;

        smartBuffer.reset();
        float dx = (clientPos.first + clientRadius) - foodPos.first;
        float dy = (clientPos.second + clientRadius) - foodPos.second;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= (clientRadius)) {
            float newSize = client.getSize() + (1.0f / std::sqrt(client.getSize()));
            client.setSize(newSize);
            client.setTextSize(client.getSize() + 40);
            smartBuffer << static_cast<int16_t>(OpCode::REMOVE_FOOD)
                        << static_cast<int16_t>(foodId)
                        << static_cast<int16_t>(Map::get().getId())
                        << static_cast<int16_t>(foodPos.first)
                        << static_cast<int16_t>(foodPos.second)
                        << static_cast<int16_t>(clientId)
                        << static_cast<float_t>(client.getSize())
                        << static_cast<unsigned int>(client.getTextSize());
            Map::get().removeFood(foodId);
            Server::get().sendToAllClients(smartBuffer);
        }
    }
}

void Protocol::check_player_collision(int clientId, std::map<int, Client>& clients, SmartBuffer &smartBuffer) {
    auto it1 = clients.find(clientId);
    if (it1 == clients.end()) return; 

    auto& player1 = it1->second;
    float size1 = player1.getSize();
    float radius1 = size1 / 2.0f;
    std::pair<float, float> pos1 = player1.getPosition();
    float centerX1 = pos1.first + radius1;
    float centerY1 = pos1.second + radius1;

    for (auto it2 = clients.begin(); it2 != clients.end(); ++it2) {
        if (it2->first == clientId) continue;

        auto& player2 = it2->second;
        float size2 = player2.getSize();
        float radius2 = size2 / 2.0f;
        std::pair<float, float> pos2 = player2.getPosition();
        float centerX2 = pos2.first + radius2;
        float centerY2 = pos2.second + radius2;

        float dx = centerX1 - centerX2;
        float dy = centerY1 - centerY2;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= (radius1 + radius2)) {
            if (size1 > size2) {
                player1.setSize(size1 + size2 / 2);
                player1.setTextSize(player1.getSize());
                std::cout << "Player " << clientId << " ate Player " << it2->first << std::endl;

                std::string removeData = std::to_string(OpCode::REMOVE_PLAYER) + " " +
                                         std::to_string(it2->first) + " " +
                                         std::to_string(pos2.first) + " " +
                                         std::to_string(pos2.second) + "\n";

                //Server::get().sendToAllClients(removeData);
                clients.erase(it2);
            } else if (size2 > size1) {
                player2.setSize(size2 + size1 / 2);
                player2.setTextSize(player2.getSize());
                std::cout << "Player " << it2->first << " ate Player " << clientId << std::endl;

                std::string removeData = std::to_string(OpCode::REMOVE_PLAYER) + " " +
                                         std::to_string(clientId) + " " +
                                         std::to_string(pos1.first) + " " +
                                         std::to_string(pos1.second) + "\n";
                //Server::get().sendToAllClients(removeData);
                clients.erase(it1);
                break;
            }
        }
    }
}

void Protocol::update_position(int id, std::map<int, Client>& clients, SmartBuffer &smartBuffer) {
    auto client = clients.find(id);
    float x, y;
    smartBuffer >> x >> y;
    if (client == clients.end()) {
        std::cerr << "Client not found" << std::endl;
        return;
    }
    auto clientPos = client->second.getPosition();
    clientPos.first += x * client->second.getSpeed();
    clientPos.second += y * client->second.getSpeed();
    if (clientPos.first < 0 || clientPos.first > Map::get().getWidth() || clientPos.second < 0 || clientPos.second > Map::get().getHeight()) {
        return;
    }

    check_food_collision(id, clientPos, client->second, smartBuffer);
    check_player_collision(id, clients, smartBuffer);

    client->second.setPosition(clientPos);
    smartBuffer.reset();
    smartBuffer << static_cast<int16_t>(OpCode::UPDATE_POSITION) << static_cast<int16_t>(id) << static_cast<float_t>(clientPos.first) << static_cast<float_t>(clientPos.second);
    Server::get().sendToAllClients(smartBuffer);
}

void Protocol::handle_message(int id, int clientSocket, std::map<int, Client>& clients, SmartBuffer& smartBuffer) {
    try {
        if (smartBuffer.getSize() < sizeof(int16_t)) {
            std::cerr << "[Protocol] Received invalid message with insufficient size." << std::endl;
            return;
        }
        int16_t opCode;
        smartBuffer >> opCode;

        switch (static_cast<OpCode>(opCode)) {
        case DEFAULT:
            std::cout << "[Protocol] DEFAULT OpCode received. No action taken." << std::endl;
            break;

        case CREATE_PLAYER:
            std::cout << "[Protocol] CREATE_PLAYER OpCode received." << std::endl;
            Protocol::get().create_player(id, clients, smartBuffer);
            break;

        case UPDATE_POSITION:
            std::cout << "[Protocol] UPDATE_POSITION OpCode received." << std::endl;
            Protocol::get().update_position(id, clients, smartBuffer);
            break;

        default:
            break;
        }

    } catch (const std::exception &e) {
        std::cerr << "[Server] Exception in client thread: " << e.what() << std::endl;
        return;
    }
}

Protocol::~Protocol() {}