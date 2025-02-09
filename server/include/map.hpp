/*
** EPITECH PROJECT, 2025
** Agario
** File description:
** map.hpp
*/

#pragma once

#include <map>
#include <utility>


class Map {
    public:
        static Map& get();
        void addFood(int id, const std::pair<int, int> &position);
        void removeFood(std::pair<int, int> foodPosition);

        const std::map<std::pair<int, int>, int> &getMap() const;
        void createMap(int id);

        int getWidth();
        int getHeight();
        int getId() const;

    private:
        Map();
        ~Map();
        int _id = 0;
        int _width = 5000;
        int _height = 5000;
        std::map<std::pair<int, int>, int> _map;
};
