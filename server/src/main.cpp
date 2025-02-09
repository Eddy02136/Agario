/*
** EPITECH PROJECT, 2024
** Agario
** File description:
** main.cpp
*/

#include <iostream>
#include "server.hpp"

int main() {
  try {
    Server::get().init();
    Server::get().run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 84;
  }
  return 0;
}