/*
** EPITECH PROJECT, 2025
** Agario
** File description:
** map.cpp
*/

#include <random>
#include "map.hpp"

Map::Map() {}

Map::~Map() {}

Map &Map::get() {
    static Map instance;
    return instance;
}

void Map::createMap(int id) {
    _id = id;
    int numCells = 50;
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
    _map[foodPosition] = id;
}

void Map::removeFood(std::pair<int, int> foodPosition) {
    _map.erase(foodPosition);
}

const std::map<std::pair<int, int>, int> &Map::getMap() const {
    return _map;
}

int Map::getId() const {
    return _id;
}

int Map::getWidth() {
    return this->_width;
}

int Map::getHeight() {
    return this->_height;
}