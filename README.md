# Agario

## Description

This project is a recreation of the Agar.io game using our own custom game engine. Inspired by the classic multiplayer arcade genre, players control a cell that grows by consuming smaller cells while avoiding larger opponents in a competitive online environment.

---

## Table of contents

1. [Prerequisites](#prerequisites)
2. [Run the game](#run-the-game)
3. [Documentation](#documentation)
4. [Project structure](#project-structure)

---

## Prerequisites

Before getting started, make sure you have the following installed on your machine:

* A C++ compiler supporting **C++17**
* CMake ≥ 3.15
* Git

Clone the repository:

```bash
git clone --recurse-submodules git@github.com:Eddy02136/Agario.git
cd Agario
```

### System libraries

Depending on your OS, install the following **development libraries**:

#### **Ubuntu / Debian**

```bash
sudo apt update
sudo apt install build-essential cmake libudev-dev \
libopenal-dev libvorbis-dev libogg-dev libflac-dev libfreetype6-dev
```

#### **Fedora**

```bash
sudo dnf install gcc-c++ cmake libudev-devel \
openal-soft-devel libvorbis-devel libogg-devel flac-devel freetype-devel
```

#### **Arch Linux**

```bash
sudo pacman -S base-devel cmake libudev openal libvorbis libogg flac freetype2
```

> These packages cover SFML dependencies for **Audio and Graphics** (OpenAL, Vorbis, FLAC, FreeType).

---

## Build the project

```bash
cmake -B build .
cd build/
make
```

---

## Run the game

### Start the Server

```bash
./build/server/Server
```

### Start the Client

Open a new terminal and make sure to be in the `Agario/` directory.

* With custom IP and port:

```bash
./build/client/Client -p 8080 -h "127.0.0.1"
```

* Without arguments (defaults to `127.0.0.1:8080`):

```bash
./build/client/Client
```

---

## Documentation

The documentation is built with **mdBook**.

### Install mdBook

#### Using Rust/Cargo (recommended)

1. Install Rust and Cargo if not installed:

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env
```

2. Install mdBook:

```bash
cargo install mdbook --version 0.5.2
```

3. Verify installation:

```bash
mdbook --version
# Should output: mdbook v0.5.2
```

### Run the documentation server

```bash
cd docs
mdbook serve
```

Open your browser at: [http://localhost:3000](http://localhost:3000)
This will provide an interactive version of the documentation.

---

## Project structure

* **client/**: Contains the client-side game logic and graphics.
* **docs/**: Contains the project documentation.
* **external/**: External libraries needed.
* **font/**: Text font resources.
* **game_engine/**: Handles server-side game engine logic.
* **server/**: Handles server-side networking.
