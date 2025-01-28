
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <components/Shape.hpp>
#include <components/Color.hpp>
#include <components/Position.hpp>
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

void Network::connectToServer() {
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
}

std::map<int, GameEngine::Entity> Network::getEntities() const {
    return _entities;
}

void Network::handleSelect() {
    fd_set readfds;
    fd_set writefds;
    int ret;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(_socket, &readfds);
    FD_SET(_socket, &writefds);
    ret = select(_socket + 1, &readfds, &writefds, NULL, NULL);
    if (ret == -1) {
        throw std::runtime_error("Failed to select");
    }
    if (ret) {
        if (FD_ISSET(_socket, &readfds)) {
            std::string data = receiveData();
            std::vector<std::string> datas = splitString(data, '\n');
            for (const auto &line : datas) {
                if (line.empty()) {
                    continue;
                }
                if (line.compare(0, 15, "PLAYER_CALLBACK") == 0) {
                    std::vector<std::string> args = splitString(line, ' ');
                    std::cout << "Callback" << std::endl;
                    if (args.size() == 2) {
                        int id = std::stoi(args[1]);
                        _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, 30), Color({133, 6, 6, 255}), Position({{0, 0}}));
                    }
                }
                if (line.compare(0, 16, "PLAYER_BROADCAST") == 0) {
                    std::cout << "Broadcast" << std::endl;
                    std::vector<std::string> args = splitString(line, ' ');
                    if (args.size() == 2) {
                        int id = std::stoi(args[1]);
                        _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, 30), Color({133, 6, 6, 255}), Position({{100, 50}}));
                    }
                }
            }
        }
    }
}

std::string Network::receiveData() {
    char buffer[1024] = {0};
    std::string data;
    if (recv(_socket, buffer, 1024, 0) < 0) {
        throw std::runtime_error("Failed to receive data");
    }
    buffer[1023] = '\0';
    data = buffer;
    return data;
}

Network::~Network() {
    close(_socket);
}