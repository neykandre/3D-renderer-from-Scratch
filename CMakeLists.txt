cmake_minimum_required(VERSION 3.30)
project(3d-renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /GR-)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -fno-rtti)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(eigen)
add_subdirectory(SFML)

add_executable(${PROJECT_NAME}
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
)

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System eigen)
