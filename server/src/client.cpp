//
// Created by Eddy Gardes on 27/01/2025.
//


#include "client.hpp"

Client::Client(int socket) : _socket(socket) {
    
}

int Client::getSocket() const {
    return this->_socket;
}

void Client::setSocket(int socket) {
    this->_socket = socket;
}

Client::~Client() {}