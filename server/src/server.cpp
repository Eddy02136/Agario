/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-eddy.gardes
** File description:
** server.cpp
*/

#include <iostream>
#include <config.hpp>
#include <sys/select.h>
#include <sstream>
#include "server.hpp"

Server::Server() {}

Server::~Server() {}

void Server::init() {
	int opt = 1;

	this->_tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_tcpSocket == FAILURE)
    	throw std::runtime_error("Failed to create TCP socket");

  	if (setsockopt(_tcpSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < SUCCESS) {
    	throw std::runtime_error( "[TCP Socket] Failed to set socket options (SO_REUSEADDR).");
	}
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
}

void Server::manage_file_descriptors() {
    FD_ZERO(&this->rfds);
    FD_ZERO(&this->wfds);
    FD_SET(this->_tcpSocket, &this->rfds);
    FD_SET(this->_tcpSocket, &this->wfds);
    for (auto &client : this->_clients) {
        if (client.second.getSocket() != FAILURE) {
            FD_SET(client.second.getSocket(), &this->rfds);
            FD_SET(client.second.getSocket(), &this->wfds);
        }
    }
}

std::string Server::get_connected_clients() const {
    std::string client_list = "PLAYER_CALLBACK " + std::to_string(this->id) + "\n";
    for (const auto &client : this->_clients) {
        client_list += "PLAYER_BROADCAST " + std::to_string(client.first) + "\n";
    }
    return client_list;
}

void Server::serialize (
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

std::string Server::deserialize(std::istream &in, char key) {
    size_t size;
    in.read(reinterpret_cast<char *>(&size), sizeof(size));
    std::vector<char> buffer(size);
    in.read(buffer.data(), static_cast<std::streamsize>(size));
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key;
    }
    return std::string(buffer.begin(), buffer.end());
}

void Server::sendToClient(int client_socket, const std::string &msg) {
    std::ostringstream out;
    this->serialize(msg, out, this->_key);
    std::string serialized = out.str();
    std::cout << "Sending message to client: " << serialized << std::endl;
    if (send(client_socket, serialized.c_str(), serialized.size(), 0) < 0) {
        std::cerr << "Failed to send message to client." << std::endl;
    }
}

void Server::add_client() {
    int client_socket = accept(this->_tcpSocket, NULL, NULL);
    if (client_socket < 0) {
        std::cerr << "Failed to accept new client." << std::endl;
    } else {
        Client new_client(client_socket);
        std::cout << "New client connected. Id = " << this->id << std::endl;
        std::string connected_clients = get_connected_clients();
        sendToClient(client_socket, connected_clients);
        std::string new_client_msg = "PLAYER_BROADCAST " + std::to_string(this->id) + "\n";
        for (auto &client : this->_clients) {
            if (client.second.getSocket() != client_socket) {
                sendToClient(client.second.getSocket(), new_client_msg);
            }
        }
		this->_clients.insert(std::make_pair(this->id, new_client));
        this->id += 2;
    }
}

void Server::run() {
    int result;
    while (true) {
        try {
		    manage_file_descriptors();
		    result = select(FD_SETSIZE, &this->rfds, &this->wfds, NULL, NULL);
    	    if (result <= SUCCESS) {
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