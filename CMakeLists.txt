cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5")
project(roguelike-samurai)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-w)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use FetchContent to get SFML
include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
)
# SFML options
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

# Add executable
add_executable(${PROJECT_NAME}
    game.cpp
    src/Game.cpp
    src/GameState.cpp
    src/Utils/FileWatcher.cpp
    engine/ECS/ECSManager.cpp
    engine/ECS/Systems/InputSystem.cpp
    engine/ECS/Systems/MovementSystem.cpp
    engine/ECS/Systems/RenderSystem.cpp
    engine/ECS/Systems/ResizeSystem.cpp
    engine/ECS/Systems/ConfigSystem.cpp
)

# Link SFML libraries (using targets from FetchContent)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System)

# Live++ Integration
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/LivePP/LivePP/API/x64" ${CMAKE_CURRENT_SOURCE_DIR})

# Compile/Link flags for Live++ with MSVC/Clang-CL
if(MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # /Z7 = Debug info in .obj (robust for hot reload)
    # /Gw = Function-level linking (similar to -ffunction-sections)
    # /Gy = Function-level linking
    # /hotpatch = Enable hotpatching padding
    target_compile_options(${PROJECT_NAME} PRIVATE /std:c++20 /Z7 /Gw /Gy -Xclang -mno-constructor-aliases)

    # Check if the compiler supports -hotpatch or /hotpatch
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
         target_compile_options(${PROJECT_NAME} PRIVATE -fms-hotpatch)
    else()
         target_compile_options(${PROJECT_NAME} PRIVATE /hotpatch)
    endif()

    # Linker flags
    # /FUNCTIONPADMIN = Required for Live++ hotpatching
    # /OPT:NOREF /OPT:NOICF = Keep symbols for live editing
    target_link_options(${PROJECT_NAME} PRIVATE "/FUNCTIONPADMIN" "/OPT:NOREF" "/OPT:NOICF" "/DEBUG:FULL")
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
