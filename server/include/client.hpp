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
        std::string getName() const;
        void setName(std::string name);
        void setSocket(int socket);
        std::pair<float, float> getPosition() const;
        void setPosition(std::pair<float, float> pos);
        float getSpeed() const;
    private:
        std::string _name = "";
        int _socket = -1;
        std::pair<float, float> _position;
        float speed = 0.5;
};
