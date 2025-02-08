# Agario

## Description

This project is a recreation of the Agar.io game using our own custom game engine.
Inspired by the classic multiplayer arcade genre, players control a cell that grows by consuming smaller cells while avoiding larger opponents in a competitive online environment.

---

## Table of contents

-   [Prerequisites](#Prerequisites)
-   [Run the game](#run-the-game)
-   [Project structure](#project-structure)
-   [Documentation](#documentation)

---

## Prerequisites

Before getting started, make sure you have the following installed on your machine:
- A C++ compiler supporting C++17

1. Clone the repository:

    ```bash
    git clone --recurse-submodules git@github.com:Eddy02136/Agario.git
    cd Agario
    ```
   
2. Install UDev library (if necessary)

   On ubuntu:

   ```bash
   sudo apt-get install libudev-dev
   ```
   
   On fedora:
   ```bash
   sudo dnf install libudev-devel
   ```

3. Build the project:
    ```bash
    cmake -B build .
    cd build/
    make
    ```
---

## Run the game

1. Start the Server:

    Make sure to be in **Agario/**

    ```bash
    ./build/server/Server
    ```

2. Start the Client:

    Open a new terminal and make sure to be in **Agario/**

    ```bash
    ./build/client/Client
    ```
---

## Project structure

`font/`: Text font.

`client/`: Contains the client-side game logic and graphics.

`external/`: External library needed.

`game_engine/`: Handles server-side game_engine.

`server/`: Handles server-side networking.

## Documentation

To access the project documentation, use `mdbook`.

1. Navigate to the `docs/` directory:
    ```bash
    cd docs
    ```
2. Start the documentation server:
    ```bash
    mdbook serve
    ```
3. Open your browser and go to:
    ```
    http://localhost:3000
    ```

This will provide an interactive version of the documentation accessible from your web browser.
