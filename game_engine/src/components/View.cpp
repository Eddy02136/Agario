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

View::View(sf::View &view, std::pair<float, float> size) {
    this->view = view;
    this->_size = size;
}

void View::setCenter(const sf::Vector2f &position)
{
    this->view.setCenter(position);
}

void View::setSize(const sf::Vector2f &size)
{
    this->_size = {size.x, size.y};
    this->view.setSize(size);
}

std::pair<float, float> View::getSize() const
{
    return this->_size;
}

std::pair<float, float> View::getCenter() const
{
    return {this->view.getCenter().x, this->view.getCenter().y};
}

sf::View &View::getView()
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