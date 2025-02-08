/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** View.hpp
*/

#pragma once

#include <string>
#include "Components.hpp"

class View : public Component {
  public:
    View(sf::View &view, std::pair<float, float> size);
    ~View();
    void setCenter(const sf::Vector2f &position);
    sf::View &getView();
    void setSize(const sf::Vector2f &size);
    std::pair<float, float> getSize() const;
    std::pair<float, float> getCenter() const;
    void display() const override;

    private:
        sf::View view;
        std::pair<float, float> _size;
};
