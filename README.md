# Agario

## Description

This project is a recreation of the Agar.io game using our own custom game engine.
Inspired by the classic multiplayer arcade genre, players control a cell that grows by consuming smaller cells while avoiding larger opponents in a competitive online environment.

---

## Table of contents

-   [Prerequisites](#Prerequisites)
-   [Run the game](#run-the-game)
-   [Documentation](#documentation)
-   [Project structure](#project-structure)

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

    You can specify the server IP address and port as arguments:

    ```bash
    ./build/client/Client -p 8080 -h "127.0.0.1"
    ```

    Or you can run the client without arguments:

    ```bash
    ./build/client/Client
    ```

    If you run the client without arguments, by default it will connect to the server at ip 127.0.0.1 and port 8080.

---

## Documentation

To access the project documentation, use `mdbook`.

### Installing `mdbook` on Ubuntu

1. Install `cargo` (if not already installed):
    ```bash
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
    ```
    Then reload your shell:
    ```bash
    source $HOME/.cargo/env
    ```

2. Install `mdbook`:
    ```bash
    cargo install mdbook
    ```

3. Verify the installation:
    ```bash
    mdbook --version
    ```

### Running the documentation server

1. Navigate to the `doc/` directory:
    ```bash
    cd doc
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

---

## Project structure

`client/`: Contains the client-side game logic and graphics.

`docs/`: Contains the project documentation.

`external/`: External library needed.

`font/`: Text font.

`game_engine/`: Handles server-side game_engine.

`server/`: Handles server-side networking.