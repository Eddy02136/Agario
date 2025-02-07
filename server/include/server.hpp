/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-eddy.gardes
** File description:
** server.hpp
*/

#pragma once

#include <netinet/in.h>
#include <map>
#include "client.hpp"
#include <SmartBuffer.hpp>
#include <mutex>

class Server {
  public:
    static Server& get();
    void init();
    void run();
    void manage_file_descriptors();
    std::map<int, Client>& getClients();
    void add_client();
    void sendToClient(int client_socket, SmartBuffer &smartBuffer);
    std::string receiveFromClient(int clientSocket);
    void serialize ( const std::string &str, std::ostream &out, char key);
    std::string deserialize(std::istream &in, char key);
    void sendToAllClients(SmartBuffer &smartBuffer);
    void sendToAllClientsExcept(int client_id, SmartBuffer &smartBuffer);
    void handle_client(int id, int clientSocket);
    fd_set& getRds();
  private:
    Server();
    ~Server();
    std::map<int, Client> _clients;
    int _tcpSocket = -1;
    int id = 0;
    fd_set rfds;
    sockaddr_in _tcpAddr{};
    std::mutex _clientMutex;
};
