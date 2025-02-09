/*
** EPITECH PROJECT, 2024
** Agario
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
    std::vector<int>& getRemoveClients();
    std::mutex& getClientMutex();
    void add_client();
    void sendToClient(int client_socket, SmartBuffer &smartBuffer);
    void setId(int id);
    int getId();
    void sendToAllClients(SmartBuffer &smartBuffer);
    void sendToAllClientsExcept(int client_id, SmartBuffer &smartBuffer);
    void handle_client(int id, int clientSocket);
    fd_set& getRds();
  private:
    Server();
    ~Server();
    std::map<int, Client> _clients;
    std::vector <int> _removeClients;
    int _tcpSocket = -1;
    int id = 0;
    fd_set rfds;
    sockaddr_in _tcpAddr{};
    std::mutex _clientMutex;
};
