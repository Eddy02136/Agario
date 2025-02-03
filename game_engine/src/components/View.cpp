/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** View.cpp
**
** Manages the camera view positioning relative to a target entity.
*/

#include <iostream>
#include "components/View.hpp"

View::View(sf::View &view) {
    this->view = view;
}

void View::setCenter(const sf::Vector2f &position)
{
    this->view.setCenter(position);
}

const sf::View &View::getView() const
{
    return this->view;
}

/**
 * @brief Display the Texture component information.
 *
 * This function displays a message indicating that the texture component is
 * being displayed.
 */
void View::display() const {
    std::cout << "View component displayed!" << std::endl;
}

View::~View() {}