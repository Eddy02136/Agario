/*
** EPITECH PROJECT, 2024
** Agario
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

static void getArgs(int ac, char **av) {
    std::cout << ac << std::endl;
	if (ac != 5) {
		return;
	}
    for (int i = 0; i < ac; i++) {
        try {
            std::string arg = av[i];

            if (arg == "-p" && i + 1 < ac) {
                Game::get().setPort(std::stoi(av[i + 1]));
            }
            if (arg == "-h" && i + 1 < ac) {
                Game::get().setIp(av[i + 1]);
            }
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

int main(int ac, char **av) {
	try {
		getArgs(ac, av);
		Game::get().gameManager();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 84;
	}
	return 0;
}
