
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
#include <clientProtocol.hpp>
#include "System.hpp"
#include "network.hpp"

Network::Network() : _ip("127.0.0.1"), _port(8080) {}

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
    smartBuffer << static_cast<int16_t>(Protocol::CREATE_PLAYER);
    smartBuffer << name;
    sendData(smartBuffer);
}

int Network::getSocket() const {
    return _socket;
}

void Network::handleMessages(std::pair<float, float> direction) {
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
                case Protocol::CREATE_PLAYER_CALLBACK:
                    std::cout << "[Network] Create Player" << std::endl;
                    Protocol::get().createPlayerCallback(smartBuffer);
                    break;
                
                case Protocol::CREATE_PLAYER_BROADCAST:
                    std::cout << "[Network] Create Player Broadcast" << std::endl;
                    Protocol::get().createPlayerBroadcast(smartBuffer);
                    break;
                
                case Protocol::UPDATE_POSITION:
                    std::cout << "[Network] Update Position" << std::endl;
                    Protocol::get().updatePosition(smartBuffer);
                    break;

                case Protocol::CREATE_MAP:
                    std::cout << "[Network] Create Map" << std::endl;
                    Protocol::get().createMap(smartBuffer);
                    break;

                case Protocol::ADD_FOOD:
                    std::cout << "[Network] Add Food" << std::endl;
                    Protocol::get().addFood(smartBuffer);
                    break;

                case Protocol::REMOVE_FOOD:
                    std::cout << "[Network] Eat Food" << std::endl;
                    Protocol::get().eatFood(smartBuffer);
                    break;

                case Protocol::EAT_PLAYER:
                    std::cout << "[Network] Eat Player" << std::endl;
                    Protocol::get().eatPlayer(smartBuffer);
                    break;

                case Protocol::REMOVE_PLAYER:
                    std::cout << "[Network] Remove Player" << std::endl;
                    Protocol::get().removePlayer(smartBuffer);
                    break;
                    
                default:
                    std::cout << "[Network] Unknown operation code: " << opCode << std::endl;
                    break;
            }
        }
        if (FD_ISSET(_socket, &writefds)) {
            smartBuffer.reset();
            smartBuffer << static_cast<int16_t>(4) << static_cast<float_t>(direction.first) << static_cast<float_t>(direction.second);
            sendData(smartBuffer);
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

void Network::sendData(SmartBuffer &smartBuffer) {
    if (send(_socket, smartBuffer.getBuffer(), smartBuffer.getSize(), 0) <= 0) {
        throw std::runtime_error("Failed to send data");
    }
}

Network::~Network() {}