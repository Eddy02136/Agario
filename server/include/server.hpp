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

class Server {
  public:
    Server();
    ~Server();
    static Server& get();
    void init();
    void run();
    void manage_file_descriptors();
    std::map<int, Client>& getClients();
    void add_client();
    void sendToClient(int client_socket, const std::string &msg);
    std::string receiveFromClient(int clientSocket);
    void serialize ( const std::string &str, std::ostream &out, char key);
    std::string deserialize(std::istream &in, char key);
    void sendToAllClients(const std::string &msg);
    void sendToAllClientsExcept(int client_id, const std::string &msg);
  private:
    std::map<int, Client> _clients;
    int _tcpSocket = -1;
    char _key = 0xA3;
    int id = 0;
    fd_set rfds;
    fd_set wfds;
    sockaddr_in _tcpAddr{};
};
