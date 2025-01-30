/*
** EPITECH PROJECT, 2025
** Agario
** File description:
** map.cpp
*/

#include "map.hpp"

Map::Map(int width, int height) : _width(width), _height(height) {}

Map::~Map() {}

void Map::addEntity(int id, const std::pair<int, int> &position) {
    _positions[id] = position;
}

void Map::removeEntity(int id) {
    _positions.erase(id);
}

const std::unordered_map<int, std::pair<int, int>> &Map::getPositions() const {
    return _positions;
}

std::pair<int, int> Map::getEntityPosition(int id) const {
    auto it = _positions.find(id);
    if (it != _positions.end()) {
        return it->second;
    }
    return {0, 0};
}

int Map::getWidth() {
    return this->_width;
}

int Map::getHeight() {
    return this->_height;
}