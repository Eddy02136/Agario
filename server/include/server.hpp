/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-eddy.gardes
** File description:
** server.hpp
*/

#pragma once

#include <netinet/in.h>
#include <unordered_map>
#include "client.hpp"

class Server {
  public:
    Server();
    ~Server();
    void init();
    void run();
    void manage_file_descriptors();
    void add_client();
    std::string get_connected_clients() const;
  private:
    std::unordered_map<int, Client> _clients;
    int _tcpSocket = -1;
    int id = 0;
    fd_set rfds;
    fd_set wfds;
    sockaddr_in _tcpAddr{};
};
