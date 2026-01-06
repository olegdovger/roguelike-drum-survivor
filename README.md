# Roguelike Samurai

A roguelike game built with C++20 and SFML 3.0 featuring an Entity Component System (ECS) architecture.

## Features

- **Entity Component System (ECS)** architecture for flexible game logic
- **SFML 3.0** for graphics and window management
- Multiple game systems including:
  - Input handling
  - Movement
  - Rendering
  - Window resize handling
- Modern C++20 codebase

## Requirements

- CMake 3.20 or higher
- C++20 compatible compiler (MSVC, GCC, or Clang)
- SFML 3.0

### Installing SFML 3.0

**Windows (MSVC/MinGW):**
Download and install SFML 3.0 from the [official website](https://www.sfml-dev.org/download.php).
The CMake configuration looks for SFML at:
- `C:/SFML-3.0.0`
- `C:/msys64/ucrt64`
- `C:/msys64/mingw64`
- `C:/msys64/usr`

Alternatively, set the `SFML_ROOT` environment variable to your SFML installation directory.

**Linux:**
```bash
sudo apt-get install libsfml-dev
```

**macOS:**
```bash
brew install sfml
```

## Building

### Windows

Using MinGW:
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

Using MSVC:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Linux/macOS

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The executable will be located in `build/bin/` directory.

## Running

After building, run the executable:
```bash
cd build/bin
./roguelike-samurai
```

## Project Structure

```
roguelike-samurai/
├── src/
│   ├── ECS/
│   │   ├── ECSManager.cpp/h    # Main ECS coordinator
│   │   ├── Entity.h            # Entity type definition
│   │   ├── Component.h         # Base component type
│   │   ├── Components.h        # All component definitions
│   │   ├── System.h            # Base system type
│   │   └── Systems/            # System implementations
│   │       ├── InputSystem.cpp/h
│   │       ├── MovementSystem.cpp/h
│   │       ├── RenderSystem.cpp/h
│   │       └── ResizeSystem.cpp/h
│   ├── Game.cpp/h              # Main game class
│   ├── GameState.h             # Game state definitions
│   └── Events.h                # Event definitions
├── resources/
│   └── fonts/                  # Game fonts
├── scripts/                    # Build and utility scripts
├── CMakeLists.txt              # CMake configuration
└── game.cpp                    # Entry point
```

## Controls

- Arrow keys: Move the player
- ESC: Close the game

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Copyright (c) 2026 Oleg Dovger

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
