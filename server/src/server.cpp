/*
** EPITECH PROJECT, 2024
** Agario
** File description:
** server
*/

#include <iostream>
#include <sys/select.h>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include "map.hpp"
#include "server.hpp"
#include "protocol.hpp"

static const int PORT = 8080;

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

int Server::getId() {
    return this->id;
}

std::mutex& Server::getClientMutex() {
    return this->_clientMutex;
}

std::vector<int>& Server::getRemoveClients() {
    return this->_removeClients;
}

void Server::setId(int id) {
    this->id = id;
}

void Server::init() {
    int opt = 1;

    this->_tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_tcpSocket == -1)
        throw std::runtime_error("Failed to create TCP socket");

    _tcpAddr.sin_family = AF_INET;
    _tcpAddr.sin_addr.s_addr = INADDR_ANY;
    _tcpAddr.sin_port = htons(PORT);
    if (bind(_tcpSocket, reinterpret_cast<sockaddr*>(&_tcpAddr), sizeof(_tcpAddr)) < 0) {
        throw std::runtime_error("Bind failed for TCP socket.");
    }

    if (listen(_tcpSocket, 3) < 0) {
        throw std::runtime_error("Listen failed for TCP socket.");
    }

    FD_ZERO(&this->rfds);
    FD_SET(this->_tcpSocket, &this->rfds);

    std::cout << "[TCP Socket] Successfully initialized." << std::endl;
    Map::get().createMap(this->id);
    this->id++;
    std::cout << "[Server] Map Successfully created." << std::endl;
}

void Server::manage_file_descriptors() {
    FD_ZERO(&this->rfds);
    FD_SET(this->_tcpSocket, &this->rfds);
    std::lock_guard<std::mutex> lock(_clientMutex);
    for (auto &client : this->_clients) {
        if (client.second.getSocket() != -1) {
            FD_SET(client.second.getSocket(), &this->rfds);
        }
    }
}

void Server::sendToClient(int client_socket, SmartBuffer &smartBuffer) {
    uint32_t size = smartBuffer.getSize();
    std::vector<uint8_t> data(sizeof(uint32_t) + size);

    std::memcpy(data.data(), &size, sizeof(uint32_t));
    std::memcpy(data.data() + sizeof(uint32_t), smartBuffer.getBuffer(), size);
    if (send(client_socket, data.data(), data.size(), 0) < 0) {
        throw std::runtime_error("Failed to send data to client.");
    }
}

void Server::sendToAllClients(SmartBuffer &smartBuffer) {
    if (this->_clients.empty()) {
        std::cerr << "[Server] No clients connected." << std::endl;
        return;
    }
    for (auto &client : this->_clients) {
        sendToClient(client.second.getSocket(), smartBuffer);
    }
}

void Server::sendToAllClientsExcept(int client_id, SmartBuffer &smartBuffer) {
    for (auto &client : this->_clients) {
        if (client.first != client_id) {
            sendToClient(client.second.getSocket(), smartBuffer);
        }
    }
}

void Server::add_client() {
    int client_socket = accept(this->_tcpSocket, NULL, NULL);
    if (client_socket < 0) {
        std::cerr << "[Server] Failed to accept new client." << std::endl;
        return;
    }
    std::cout << "[Server] New client socket accepted: " << client_socket << std::endl;
    Client new_client(client_socket);
    std::lock_guard<std::mutex> lock(_clientMutex);
    this->_clients.insert({this->id, new_client});
    std::cout << "[Server] New client connected with id: " << this->id << std::endl;
    std::thread clientThread(&Server::handle_client, this, this->id, client_socket);
    clientThread.detach();
    this->id += 2;
}

void Server::handle_client(int id, int clientSocket) {
    try {
        SmartBuffer smartBuffer;
        while (true) {
            if (FD_ISSET(clientSocket, &this->rfds)) {
                char buffer[1024] = {};

                const ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesRead <= 0) {
                    std::cerr << "[TCP Socket] Client disconnected: " << id << std::endl;
                    std::lock_guard<std::mutex> lock(_clientMutex);
                    if (_clients.find(id) != _clients.end()) {
                        _clients.erase(id);
                    }
                    FD_CLR(clientSocket, &this->rfds);
                    shutdown(clientSocket, SHUT_RDWR);
                    ::close(clientSocket);
                    return;
                }
                smartBuffer.reset();
                smartBuffer.inject(reinterpret_cast<uint8_t*>(buffer), bytesRead);
                Protocol::get().handle_message(id, clientSocket, this->_clients, smartBuffer);
            }
            for (int clientIndex : this->_removeClients) {
                std::lock_guard<std::mutex> lock(_clientMutex);
                auto client = _clients.find(clientIndex);
                if (client != _clients.end()) {
                    FD_CLR(client->second.getSocket(), &this->rfds);
                    shutdown(client->second.getSocket(), SHUT_RDWR);
                    ::close(client->second.getSocket());
                    _clients.erase(client->first);
                    return;
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
            result = select(FD_SETSIZE, &this->rfds, NULL, NULL, NULL);
            if (result < 0) {
                throw std::runtime_error("Select failed.");
            }
            if (FD_ISSET(this->_tcpSocket, &this->rfds)) {
                add_client();
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return;
        }
    }
}