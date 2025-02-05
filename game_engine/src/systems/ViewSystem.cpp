/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-anastasia.bouby
** File description:
** Implements the `viewSystem` function, which manages the rendering 
** and camera system within the game engine.
** Responsibility:
** - Adjust the camera position based on player movement
*/

#include <components/Position.hpp>
#include <components/View.hpp>
#include "System.hpp"

void GameEngine::System::viewSystem(sf::RenderWindow& window,
                                    GameEngine::Entity& entity) {
    if (entity.hasComponent<View>() && entity.hasComponent<Position>()) {
        entity.getComponent<View>().setCenter({
            entity.getComponent<Position>().getPositions()[0].first, 
            entity.getComponent<Position>().getPositions()[0].second});
        entity.getComponent<View>().setSize({
            entity.getComponent<View>().getSize().first, 
            entity.getComponent<View>().getSize().second});
        window.setView(entity.getComponent<View>().getView());
    }
}