/*
** EPITECH PROJECT, 2025
** Agario
** File description:
** map.hpp
*/

#pragma once

#include <unordered_map>
#include <utility>


class Map {
    public:
        Map(int width, int height);
        ~Map();

        void addEntity(int id, const std::pair<int, int> &position);
        void removeEntity(int id);

        const std::unordered_map<int, std::pair<int, int>> &getPositions() const;
        std::pair<int, int> getEntityPosition(int id) const;

        int getWidth();
        int getHeight();

    private:
        int _width = 0;
        int _height = 0;
        std::unordered_map<int, std::pair<int, int>> _positions;
};
