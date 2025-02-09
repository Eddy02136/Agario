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
#include <unistd.h>
#include <sys/select.h>
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
    std::map<std::pair<int, int>, int> map = Map::get().getMap();
    for (const auto &food : map) {
        smartBuffer << static_cast<int16_t>(OpCode::ADD_FOOD)
                    << static_cast<int16_t>(Map::get().getId())
                    << static_cast<int16_t>(food.first.first)
                    << static_cast<int16_t>(food.first.second);
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
    std::map<std::pair<int, int>, int> foodMap = Map::get().getMap();
    float clientRadius = client.getSize();
    int searchRadius = static_cast<int>(clientRadius) + 10;
    
    int x1 = static_cast<int>(clientPos.first - searchRadius);
    int y1 = static_cast<int>(clientPos.second - searchRadius);
    int x2 = static_cast<int>(clientPos.first + searchRadius);
    int y2 = static_cast<int>(clientPos.second + searchRadius);

    auto lower = foodMap.lower_bound({x1, y1});
    auto higher = foodMap.upper_bound({x2, y2});
    std::cout << "Client found " << std::distance(lower, higher) << " food items" << std::endl;

    for (auto food = lower; food != higher; ++food) {
        if (food == foodMap.end()) {
            break;
        }

        int foodId = food->second;
        const auto& foodPos = food->first;

        if (std::isfinite(clientPos.first) && std::isfinite(clientPos.second) && 
            std::isfinite(foodPos.first) && std::isfinite(foodPos.second)) {
            
            float dx = (clientPos.first + clientRadius) - foodPos.first;
            float dy = (clientPos.second + clientRadius) - foodPos.second;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            if (distance <= clientRadius) {
                float newSize = client.getSize() + (1.0f / std::sqrt(client.getSize()));
                client.setSize(newSize);
                client.setTextSize(client.getSize() + 40);

                smartBuffer.reset();
                smartBuffer << static_cast<int16_t>(OpCode::REMOVE_FOOD)
                            << static_cast<int16_t>(foodId)
                            << static_cast<int16_t>(Map::get().getId())
                            << static_cast<int16_t>(foodPos.first)
                            << static_cast<int16_t>(foodPos.second)
                            << static_cast<int16_t>(clientId)
                            << static_cast<float_t>(client.getSize())
                            << static_cast<unsigned int>(client.getTextSize())
                            << static_cast<int16_t>(client.getScore());
                
                std::cout << "Player " << clientId << " ate food " << foodId << std::endl;
                Map::get().removeFood(foodPos);
                
                Server::get().sendToAllClients(smartBuffer);
            }
        }
    }
}

void Protocol::check_player_collision(int clientId, std::map<int, Client>& clients, SmartBuffer &smartBuffer) {
    auto clientIndex = clients.find(clientId);
    if (clientIndex == clients.end()) return;

    auto& client = clientIndex->second;
    float clientSize = client.getSize();
    float clientRadius = clientSize / 2.0f;
    std::pair<float, float> clientPos = client.getPosition();
    float clientCenterX = clientPos.first + clientRadius;
    float clientCenterY = clientPos.second + clientRadius;

    for (auto otherIndex = clients.begin(); otherIndex != clients.end(); otherIndex++) {
        if (otherIndex->first == clientId) continue;

        auto& otherClient = otherIndex->second;
        float otherSize = otherClient.getSize();
        float otherRadius = otherSize / 2.0f;
        std::pair<float, float> otherPos = otherClient.getPosition();
        float otherCenterX = otherPos.first + otherRadius;
        float otherCenterY = otherPos.second + otherRadius;

        float dx = clientCenterX - otherCenterX;
        float dy = clientCenterY - otherCenterY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= (clientRadius + otherRadius)) {
            if (otherSize > clientSize) {
                smartBuffer.reset();
                otherClient.setSize(otherSize + clientSize/ 2);
                otherClient.setTextSize(otherClient.getSize());
                otherClient.setScore(otherClient.getScore() + (clientSize * 2));
                smartBuffer << static_cast<int16_t>(OpCode::EAT_PLAYER)
                            << static_cast<int16_t>(otherIndex->first)
                            << static_cast<float_t>(otherClient.getSize())
                            << static_cast<unsigned int>(otherClient.getTextSize())
                            << static_cast<int16_t>(otherClient.getScore());
                Server::get().sendToAllClients(smartBuffer);
                smartBuffer.reset();
                smartBuffer << static_cast<int16_t>(OpCode::REMOVE_PLAYER)
                            << static_cast<int16_t>(clientIndex->first);
                Server::get().sendToAllClients(smartBuffer);
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
    check_food_collision(id, clientPos, client->second, smartBuffer);
    check_player_collision(id, clients, smartBuffer);
    if (clientPos.first < 0 || clientPos.first > Map::get().getWidth() || clientPos.second < 0 || clientPos.second > Map::get().getHeight()) {
        return;
    }

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