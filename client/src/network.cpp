
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <components/Shape.hpp>
#include <components/Color.hpp>
#include <components/Position.hpp>
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
    std::string name = "Guest";
    std::string data = "1 " + name + " " + "\n";
    //std::ostringstream out;
    //serialize(data, out, _key);
    send(_socket, data.c_str(), data.size(), 0);
}

std::map<int, GameEngine::Entity> Network::getEntities() const {
    return _entities;
}

void Network::serialize (
    const std::string &str, std::ostream &out, char key)
{
    size_t size = str.size();
    out.write(reinterpret_cast<const char *>(&size), sizeof(size));

    std::vector<char> buffer(str.begin(), str.end());
    for (size_t i = 0; i < size; i++) {
        buffer.data()[i] ^= key;
    }
    out.write(buffer.data(), static_cast<std::streamsize>(size));
}

std::string Network::deserialize(std::istream &in, char key) {
    size_t size = 0;
    in.clear();

    if (!in.read(reinterpret_cast<char *>(&size), sizeof(size))) {
        return "";
    }

    if (size > static_cast<size_t>(in.rdbuf()->in_avail())) {
        return "";
    }
    std::vector<char> buffer(size);
    in.read(buffer.data(), static_cast<std::streamsize>(size));
    std::cout << "Buffer: " << std::string(buffer.begin(), buffer.end()) << std::endl;
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key;
    }
    return std::string(buffer.begin(), buffer.end());
}

void Network::handleSelect(std::pair<float, float> direction) {
    fd_set readfds;
    fd_set writefds;
    GameEngine::System system;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 500000;
    int ret;

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
            std::string data = receiveData();
            std::vector<std::string> datas = splitString(data, '\n');
            for (const auto &line : datas) {
                if (line.empty()) {
                    continue;
                }
                if (line.compare(0, 1, "2") == 0) {
                    std::vector<std::string> args = splitString(line, ' ');
                    std::cout << "Callback" << std::endl;
                    if (args.size() == 4) {
                        int id = std::stoi(args[1]);
                        std::pair<float, float> pos = {std::stof(args[2]), std::stof(args[3])};
                        _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, 30), Color({133, 6, 6, 255}), Position({{pos.first, pos.second}}));
                    }
                }
                if (line.compare(0, 1, "3") == 0) {
                    std::cout << "Broadcast" << std::endl;
                    std::vector<std::string> args = splitString(line, ' ');
                    if (args.size() == 4) {
                        int id = std::stoi(args[1]);
                        std::pair<float, float> pos = {std::stof(args[2]), std::stof(args[3])};
                        _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, 30), Color({133, 6, 6, 255}), Position({{pos.first, pos.second}}));
                    }
                }
                if (line.compare(0, 1, "4") == 0) {
                    std::cout << "Update Position" << std::endl;
                    std::vector<std::string> args = splitString(line, ' ');
                    if (args.size() == 4) {
                        int id = std::stoi(args[1]);
                        std::pair<float, float> pos = {std::stof(args[2]), std::stof(args[3])};
                        system.update(id, _entities, GameEngine::UpdateType::Position, pos, 0);
                    }
                }
            }
        }
        if (FD_ISSET(_socket, &writefds)) {
            std::string data = "4 " + std::to_string(direction.first) + " " + std::to_string(direction.second) + "\n";
            //std::ostringstream out;
            //serialize(data, out, _key);
            send(_socket, data.c_str(), data.size(), 0);
        }
    }
}

std::string Network::receiveData() {
    char buffer[1024] = {0};
    std::string data;
    ssize_t bytesReceived = recv(_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0) {
        throw std::runtime_error("Failed to receive data");
    }
    buffer[bytesReceived] = '\0';
    std::cout << "Bytes received: " << bytesReceived << std::endl;
    data = std::string(buffer, bytesReceived);
    //std::istringstream in(data);
    //std::cout << "Received: " << data << std::endl;
    //data = deserialize(in, _key);
    std::cout << "Deserialized: " << data << std::endl;
    return data;
}

Network::~Network() {}