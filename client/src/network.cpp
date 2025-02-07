
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sstream>
#include <components/Shape.hpp>
#include <components/Color.hpp>
#include <components/Position.hpp>
#include <components/View.hpp>
#include <components/Text.hpp>
#include <components/Link.hpp>
#include <cmath>
#include "System.hpp"
#include "network.hpp"

Network::Network() : _ip("127.0.0.1"), _port(8080) {}

static std::vector<std::string> splitString(const std::string &data, const char delimiter) {
    std::stringstream str(data);
    std::string line;
    std::vector<std::string> result;
    while (std::getline(str, line, delimiter)) {
        result.push_back(line);
    }
    return result;
}

void Network::connectToServer(std::string &name) {
    struct sockaddr_in server;
    _socket = -1;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        throw std::runtime_error("Failed to create socket");
    }
    server.sin_addr.s_addr = inet_addr(_ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    if (connect(_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        throw std::runtime_error("Failed to connect to server");
    }
    if (name.empty()) {
        name = "Guest";
    }
    SmartBuffer smartBuffer;
    smartBuffer << static_cast<int16_t>(1);
    smartBuffer << name;
    if (send(_socket, smartBuffer.getBuffer(), smartBuffer.getSize(), 0) < 0) {
        throw std::runtime_error("Failed to send data");
    }
}

std::map<int, GameEngine::Entity> Network::getEntities() const {
    return _entities;
}

int Network::getSocket() const {
    return _socket;
}

void Network::createPlayerCallback(SmartBuffer &SmartBuffer) {
    uint16_t id, x, y, size, textSize;
    std::string name = "";
    SmartBuffer >> id >> name >> x >> y >> size >> textSize;
    sf::View view = sf::View(sf::FloatRect(0, 0, 1280, 720));
    std::pair<float, float> pos = {x, y};
    _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, size), Color({133, 6, 6, 255}), Position({{pos.first, pos.second}}), View(view, {1280, 720}));
    _entities[id + 1] = GameEngine::Entity(id + 1, Text(name, "font/Inter_Bold.ttf", 30), Position({{pos.first, pos.second}}), Link(id));
}

void Network::updatePosition(SmartBuffer &smartBuffer) {
    uint16_t id;
    float x, y;
    smartBuffer >> id >> x >> y;
    std::pair<float, float> pos = {x, y};
    GameEngine::System system;
    system.update(id, _entities, GameEngine::UpdateType::Position, pos); 
    //system.update(id + 1, _entities, GameEngine::UpdateType::Position, pos, 0);
}

void Network::createMap(SmartBuffer &smartBuffer) {
    uint16_t id;
    smartBuffer >> id;
    _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, 5), Color({173, 216, 230, 255}));
}

void Network::addFood(SmartBuffer &smartBuffer) {
    uint16_t mapid, foodid, x, y;
    smartBuffer >> mapid >> x >> y;
    if (_entities.find(mapid) != _entities.end()) {
        if (!_entities[mapid].hasComponent<Position>()) {
            _entities[mapid].addComponent(Position({{x, y}}));
        } else {
            _entities[mapid].getComponent<Position>().addPosition(x, y);
        }
    }
}

void Network::createPlayerBroadcast(SmartBuffer &SmartBuffer) {
    uint16_t id, x, y, size, textSize;
    std::string name = "";
    SmartBuffer >> id >> name >> x >> y >> size >> textSize;
    std::pair<float, float> pos = {x, y};
    _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, size), Color({133, 6, 6, 255}), Position({{pos.first, pos.second}}));
    _entities[id + 1] = GameEngine::Entity(id + 1, Text(name, "font/Inter_Bold.ttf", 30), Position({{pos.first, pos.second}}), Link(id));
}

void Network::eatFood(SmartBuffer &smartBuffer) {
    GameEngine::System system;
    uint16_t foodId, mapId, x, y, clientId;
    float size;
    unsigned int textSize;
    smartBuffer >> foodId >> mapId >> x >> y >> clientId >> size >> textSize;
    if (_entities.find(mapId) != _entities.end()) {
        std::pair<float, float> pos = {x, y};
        _entities[mapId].getComponent<Position>().removePosition(pos);
        system.update(clientId, _entities, GameEngine::UpdateType::CircleRadius, size);
        if (textSize > 0) {
            system.update(clientId + 1, _entities, GameEngine::UpdateType::TextSize, textSize);
        }
        if (_entities[clientId].hasComponent<View>()) {
            auto &viewComp = _entities[clientId].getComponent<View>();
            std::pair<float, float> viewSize = viewComp.getSize();
            float playerSize = size;
            const std::pair<float, float> V0 = {1280.0f, 720.0f};
            const float S0 = 30.0f;
            const float alpha = 0.6f;
            std::pair<float, float> newSize = {
                V0.first * std::pow(playerSize / S0, alpha),
                V0.second * std::pow(playerSize / S0, alpha)
            };
            system.update(clientId, _entities, GameEngine::UpdateType::View, newSize);
        }
    }
}

void Network::handleSelect(std::pair<float, float> direction) {
    fd_set readfds;
    fd_set writefds;
    GameEngine::System system;
    int ret;
    SmartBuffer smartBuffer;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    if (_socket == -1) {
        throw std::runtime_error("Socket not initialized before select");
    }
    if (_socket < 0) {
        throw std::runtime_error("Invalid socket in handleSelect");
    }
    if (_socket >= FD_SETSIZE) {
        throw std::runtime_error("Socket descriptor exceeds FD_SETSIZE");
    }
    FD_SET(_socket, &readfds);
    FD_SET(_socket, &writefds);
    ret = select(FD_SETSIZE, &readfds, &writefds, NULL, &timeout);
    if (ret == -1) {
        throw std::runtime_error("Failed to select");
    }
    if (ret) {
        if (FD_ISSET(_socket, &readfds)) {
            receiveData(smartBuffer);
            int16_t opCode;
            smartBuffer >> opCode;
            switch(opCode) {
                case 2:
                    std::cout << "[Network] Create Player" << std::endl;
                    createPlayerCallback(smartBuffer);
                    break;
                
                case 3:
                    std::cout << "[Network] Create Player Broadcast" << std::endl;
                    createPlayerBroadcast(smartBuffer);
                    break;
                
                case 4:
                    std::cout << "[Network] Update Position" << std::endl;
                    updatePosition(smartBuffer);
                    break;

                case 5:
                    std::cout << "[Network] Create Map" << std::endl;
                    createMap(smartBuffer);
                    break;

                case 6:
                    std::cout << "[Network] Add Food" << std::endl;
                    addFood(smartBuffer);
                    break;

                case 7:
                    std::cout << "[Network] Eat Food" << std::endl;
                    eatFood(smartBuffer);
                    break;
                    
                default:
                    std::cout << "[Network] Unknown operation code: " << opCode << std::endl;
                    break;
            }
        }
        if (FD_ISSET(_socket, &writefds)) {
            smartBuffer.reset();
            smartBuffer << static_cast<int16_t>(4) << static_cast<float_t>(direction.first) << static_cast<float_t>(direction.second);
            if (send(_socket, smartBuffer.getBuffer(), smartBuffer.getSize(), 0) <= 0) {
                throw std::runtime_error("Failed to send data");
            }
        }
    }
}

void Network::receiveData(SmartBuffer &smartBuffer) {
    uint32_t size;
    if (recv(_socket, &size, sizeof(uint32_t), 0) <= 0) {
        throw std::runtime_error("Failed to receive size");
    }
    std::vector<uint8_t> data(size);
    if (recv(_socket, data.data(), size, 0) <= 0) {
        throw std::runtime_error("Failed to receive data");
    }
    smartBuffer.reset();
    smartBuffer.inject(data.data(), size);
}

Network::~Network() {}