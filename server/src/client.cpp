/*
** EPITECH PROJECT, 2024
** Agario
** File description:
** client
*/

#include "client.hpp"
#include "unistd.h"
#include <cstdlib>
#include <ctime>

Client::Client(int socket) : _socket(socket) {
    std::srand(std::time(0));
    float x = std::rand() % 1280;
    float y = std::rand() % 720;
    this->_position = std::make_pair(x, y);
    this->_size = 30;
    this->_textSize = 10;
}

int Client::getSocket() const {
    return this->_socket;
}

std::string Client::getName() const {
    return this->_name;
}

void Client::setName(std::string name) {
    this->_name = name;
}

std::pair<float, float> Client::getPosition() const {
    return this->_position;
}

void Client::setPosition(std::pair<float, float> pos) {
    this->_position = pos;
}

void Client::setSocket(int socket) {
    this->_socket = socket;
}

float Client::getSpeed() const {
    return this->speed;
}

float Client::getSize() {
    return this->_size;
}

void Client::setSize(float size) {
    this->_size = size;
}

float Client::getTextSize() {
    return this->_textSize;
}

void Client::setTextSize(float newSize) {
    this->_textSize = newSize;
}

int Client::getScore() const {
    return this->_score;
}

void Client::setScore(int score) {
    this->_score = score;
}

Client::~Client() {}