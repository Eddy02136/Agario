
#include <iostream>
#include <config.hpp>
#include <sys/select.h>
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

std::mutex &Server::getQueueMutex() {
    return this->_queueMutex;
}

std::condition_variable &Server::getCond() {
    return this->_cond;
}

std::queue<Packet>& Server::getQueue() {
    return this->_queue;
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

void Server::sendToClient(int client_socket, const std::string &msg) {
    std::ostringstream out;
    this->serialize(msg, out, this->_key);
    std::string serialized = out.str();
    std::cout << "[Server] Sending message to client: " << serialized << std::endl;
    const size_t MAX_SIZE = 1024;
    size_t totalSent = 0;
    size_t msgLength = serialized.size();
    while (totalSent < msgLength) {
        size_t chunkSize = std::min(MAX_SIZE, msgLength - totalSent);
        ssize_t sent = send(client_socket, serialized.c_str() + totalSent, chunkSize, 0);
        if (sent < 0) {
            std::cerr << "[Server] Failed to send message to client." << std::endl;
            return;
        }
        totalSent += sent;
    }
}

std::string Server::receiveFromClient(int clientSocket, int client_id) {
    char buffer[1024] = {0};
    std::string data;
    if (clientSocket < 0) {
        return "";
    }
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        return "";
    }
    buffer[bytesReceived] = '\0';
    data = std::string(buffer, bytesReceived);
    std::istringstream in(data);
    data = deserialize(in, _key);
    std::cout << "Received: " << data << std::endl;
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
    std::lock_guard<std::mutex> lock(_clientMutex);
    this->_clients.insert({this->id, new_client});
    std::cout << "[Server] New client connected with id: " << this->id << std::endl;
    this->id += 2;
}

void Server::receiveLoop() {
    while (true) {
        try {
            manage_file_descriptors();
            int result = select(FD_SETSIZE, &this->rfds, nullptr, nullptr, nullptr);
            if (result < 0) {
                std::cerr << "Select failed." << std::endl;
                break;
            }
            if (FD_ISSET(this->_tcpSocket, &this->rfds)) {
                add_client();
            }

            std::lock_guard<std::mutex> lock(_clientMutex);
            for (auto &client : _clients) {
                int clientSocket = client.second.getSocket();
                if (FD_ISSET(clientSocket, &this->rfds)) {
                    std::string message = receiveFromClient(clientSocket, client.first);
                    if (!message.empty()) {
                        Protocol::get().handle_message(client.first, clientSocket, message);
                    } else {
                        close(clientSocket);
                        this->_clients.erase(client.first);
                        std::lock_guard<std::mutex> lockqueue(_queueMutex);
                        std::queue<Packet> tempQueue;
                        while (!_queue.empty()) {
                            Packet packet = _queue.front();
                            _queue.pop();
                            if (packet.socket != clientSocket) {
                                tempQueue.push(packet);
                            }
                        }
                        _queue = std::move(tempQueue);
                        std::cerr << "[Server] Client " << clientSocket << " disconnected." << std::endl;
                        break;
                    }
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "[Server] Exception in receiveLoop: " << e.what() << std::endl;
        }
    }
}

void Server::sendLoop() {
    while (true) {
        std::unique_lock<std::mutex> lock(_queueMutex);
        _cond.wait(lock, [this] { return !_queue.empty(); });

        while (!_queue.empty()) {
            Packet packet = _queue.front();
            _queue.pop();
            lock.unlock();
            if (packet.destination == Packet::dest::ALL) {
                sendToAllClients(packet.message);
            } else if (packet.destination == Packet::dest::OTHERS) {
                sendToAllClientsExcept(packet.socket, packet.message);
            } else {
                sendToClient(packet.socket, packet.message);
            }
            lock.lock();
        }
    }
}

void Server::run() {
    int result;
    try {
        _receiveThread = std::thread(&Server::receiveLoop, this);
        _sendThread = std::thread(&Server::sendLoop, this);
        while (true) {
             std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
    } catch (const std::exception& exception) {
        std::cerr << "[Server] Error: " << exception.what() << std::endl;
        return;
    }
}