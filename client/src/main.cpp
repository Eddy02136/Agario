/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-eddy.gardes
** File description:
** main.cpp
*/

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <map>
#include <iostream>
#include <components/Position.hpp>
#include <components/Shape.hpp>
#include <components/Link.hpp>
#include <components/Sprite.hpp>
#include <components/Text.hpp>
#include <components/Texture.hpp>
#include <components/Color.hpp>
#include "Entity.hpp"
#include "System.hpp"
#include "game.hpp"

int main() {
	try {
		Game::get().gameManager();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 84;
	}
	return 0;
}
