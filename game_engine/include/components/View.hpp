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
    View(sf::View &view);
    ~View();
    void setCenter(const sf::Vector2f &position);
    const sf::View &getView() const;
    void display() const override;

    private:
        sf::View view;
};
