/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-eddy.gardes
** File description:
** client.hpp
*/

#pragma once

#include <iostream>

class Client {
    public:
        Client(int socket);
        ~Client();
        int getSocket() const;
        void setSocket(int socket);
    private:
        int _socket = -1;
        std::pair<float, float> _position;
};
