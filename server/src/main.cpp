/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-eddy.gardes
** File description:
** main.cpp
*/

#include <iostream>
#include "config.hpp"
#include "server.hpp"

int main() {
  Server server;

  try {
    server.init();
    server.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return ERROR;
  }
  return 0;
}