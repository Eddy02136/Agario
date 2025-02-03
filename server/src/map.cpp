/*
** EPITECH PROJECT, 2025
** Agario
** File description:
** map.cpp
*/

#include "map.hpp"

Map::Map() {}

Map::~Map() {}

Map &Map::get() {
    static Map instance;
    return instance;
}

void Map::createMap(int id) {
    _id = id;
    int numCells = 100;
    int cellWidth = _width / numCells;
    int cellHeight = _height / numCells;

    for (int i = 0; i < numCells; i++) {
        for (int j = 0; j < numCells; j++) {
            int x = i * cellWidth + rand() % cellWidth;
            int y = j * cellHeight + rand() % cellHeight;
            addFood(i * numCells + j, {x, y});
        }
    }
}

void Map::addFood(int id, const std::pair<int, int> &foodPosition) {
    _map[id] = foodPosition;
}

void Map::removeFood(int id) {
    _map.erase(id);
}

const std::map<int, std::pair<int, int>> &Map::getMap() const {
    return _map;
}

int Map::getId() const {
    return _id;
}

std::pair<int, int> Map::getFoodPosition(int id) const {
    auto it = _map.find(id);
    if (it != _map.end()) {
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