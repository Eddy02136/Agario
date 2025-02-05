
#include <iostream>
#include <config.hpp>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include "map.hpp"
#include "server.hpp"
#include "protocol.hpp"

Server::Server() {}

Server& Server::get() {
    static Server instance;
    return instance;
}

Server::~Server() {
    close(this->_tcpSocket);
}

std::map<int, Client>& Server::getClients() {
    return this->_clients;
}

fd_set& Server::getRds() {
    return this->rfds;
}

fd_set& Server::getWds() {
    return this->wfds;
}

void Server::init() {
    int opt = 1;

    this->_tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_tcpSocket == FAILURE)
        throw std::runtime_error("Failed to create TCP socket");

    _tcpAddr.sin_family = AF_INET;
    _tcpAddr.sin_addr.s_addr = INADDR_ANY;
    _tcpAddr.sin_port = htons(PORT);
    if (bind(_tcpSocket, reinterpret_cast<sockaddr*>(&_tcpAddr), sizeof(_tcpAddr)) < SUCCESS) {
        throw std::runtime_error("Bind failed for TCP socket.");
    }

    if (listen(_tcpSocket, 3) < SUCCESS) {
        throw std::runtime_error("Listen failed for TCP socket.");
    }

    FD_ZERO(&this->rfds);
    FD_ZERO(&this->wfds);
    FD_SET(this->_tcpSocket, &this->rfds);
    FD_SET(this->_tcpSocket, &this->wfds);

    std::cout << "[TCP Socket] Successfully initialized." << std::endl;
    Map::get().createMap(this->id);
    this->id++;
    std::cout << "[Server] Map Successfully created." << std::endl;
}

void Server::manage_file_descriptors() {
    FD_ZERO(&this->rfds);
    FD_ZERO(&this->wfds);
    FD_SET(this->_tcpSocket, &this->rfds);
    FD_SET(this->_tcpSocket, &this->wfds);
    std::lock_guard<std::mutex> lock(_clientMutex);
    for (auto &client : this->_clients) {
        if (client.second.getSocket() != FAILURE) {
            FD_SET(client.second.getSocket(), &this->rfds);
            FD_SET(client.second.getSocket(), &this->wfds);
        }
    }
}

void Server::serialize(const std::string &str, std::ostream &out, char key) {
    size_t size = str.size();
    out.write(reinterpret_cast<const char *>(&size), sizeof(size));
    std::vector<char> buffer(str.begin(), str.end());
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key;
    }
    out.write(buffer.data(), static_cast<std::streamsize>(size));
}

std::string Server::deserialize(std::istream &in, char key) {
    size_t size = 0;
    in.clear();
    if (!in.read(reinterpret_cast<char *>(&size), sizeof(size))) {
        return "";
    }
    std::vector<char> buffer(size);
    if (!in.read(buffer.data(), static_cast<std::streamsize>(size))) {
        return "";
    }
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key;
    }
    return std::string(buffer.begin(), buffer.end());
}

void Server::sendToClient(int client_socket, const std::string &msg) {
    const size_t MAX_SIZE = 3000;
    size_t totalSent = 0;
    size_t msgLength = msg.size();
    
    while (totalSent < msgLength) {
        size_t chunkSize = std::min(MAX_SIZE, msgLength - totalSent);
        std::ostringstream out;
        this->serialize(msg.substr(totalSent, chunkSize), out, this->_key);
        std::string serialized = out.str();
        ssize_t sent = send(client_socket, serialized.c_str(), serialized.size(), 0);
        if (sent < 0) {
            std::cerr << "[Server] Failed to send message to client." << std::endl;
            return;
        }
        
        totalSent += chunkSize;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    std::cout << "[Server] Message fully sent to client." << std::endl;
}

std::string Server::receiveFromClient(int clientSocket) {
    int available = 0;
    if (ioctl(clientSocket, FIONREAD, &available) < 0) {
        throw std::runtime_error("Failed to check available data");
    }

    if (available <= 0) {
        return "";
    }

    std::vector<char> buffer(available + 1, 0);
    ssize_t bytesReceived = recv(clientSocket, buffer.data(), available, 0);
    
    if (bytesReceived < 0) {
        throw std::runtime_error("Failed to receive data");
    }

    std::cout << "Bytes received: " << bytesReceived << std::endl;

    std::string data(buffer.begin(), buffer.begin() + bytesReceived);
    std::istringstream in(data);
    
    std::cout << "Received: " << data << std::endl;
    data = deserialize(in, _key);
    std::cout << "Deserialized: " << data << std::endl;
    
    return data;
}

void Server::sendToAllClients(const std::string &msg) {
    if (this->_clients.empty()) {
        std::cerr << "[Server] No clients connected." << std::endl;
        return;
    }
    for (auto &client : this->_clients) {
        sendToClient(client.second.getSocket(), msg);
    }
}

void Server::sendToAllClientsExcept(int client_id, const std::string &msg) {
    for (auto &client : this->_clients) {
        if (client.first != client_id) {
            sendToClient(client.second.getSocket(), msg);
        }
    }
}

void Server::add_client() {
    int client_socket = accept(this->_tcpSocket, NULL, NULL);
    if (client_socket < 0) {
        std::cerr << "[Server] Failed to accept new client." << std::endl;
        return;
    }
    Client new_client(client_socket);
    {
        std::lock_guard<std::mutex> lock(_clientMutex);
        this->_clients.insert({this->id, new_client});
    }
    std::cout << "[Server] New client connected with id: " << this->id << std::endl;

    std::thread clientThread(&Server::handle_client, this, this->id, client_socket);
    clientThread.detach();
    this->id += 2;
}

void Server::handle_client(int id, int clientSocket) {
    try {
        while (true) {
            if (FD_ISSET(clientSocket, &this->rfds)) {
                bool disconnected = Protocol::get().handle_message(id, clientSocket, _clients);
                if (disconnected) {
                    std::lock_guard<std::mutex> lock(_clientMutex);
                    auto client = _clients.find(id);
                    if (client != _clients.end()) {
                        FD_CLR(clientSocket, &this->rfds);
                        FD_CLR(clientSocket, &this->wfds);
                        close(clientSocket);
                        _clients.erase(client);
                        std::cout << "test" << std::endl;
                        std::cout << "[Server] Client " << id << " disconnected." << std::endl;
                    }
                    break;
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "[Server] Exception in client thread: " << e.what() << std::endl;
    }
}

void Server::run() {
    int result;
    while (true) {
        try {
            manage_file_descriptors();
            result = select(FD_SETSIZE, &this->rfds, &this->wfds, NULL, NULL);
            if (result < SUCCESS) {
                throw std::runtime_error("Select failed.");
            }
            if (FD_ISSET(this->_tcpSocket, &this->rfds)) {
                add_client();
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            std::cout << "[Server] Shutting down." << std::endl;
            return;
        }
    }
}