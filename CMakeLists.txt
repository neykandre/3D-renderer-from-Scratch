cmake_minimum_required(VERSION 3.30)
project(3d-renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif ()

if (MSVC)
    add_compile_options(/W4 /GR-)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -fno-rtti)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(eigen)
add_subdirectory(SFML)

set(SOURCES
        src/main.cpp
        src/application/Application.cpp
        src/geometry/Triangle.cpp
        src/world/Object.cpp
        src/world/World.cpp
        src/rendering/Renderer.cpp
        src/rendering/Screen.cpp
        src/light/AmbientLight.cpp
        src/light/DirectionalLight.cpp
        src/world/Camera.cpp
        src/geometry/Vertex.cpp
        src/exceptions/Except.cpp
        src/loaders/ObjLoader.cpp
        src/light/Material.cpp
        src/light/Color.cpp
        src/ui/ConsoleInterface.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System eigen)
