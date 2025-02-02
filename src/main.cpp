
#include "rendering/Renderer.h"
#include "world/Camera.h"
#include "world/World.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main() {
    const int width  = 800;
    const int height = 600;

    sf::RenderWindow window(sf::VideoMode({ width, height }), "3D Renderer");
    window.setFramerateLimit(60);

    renderer::Renderer renderer(width, height);
    renderer::World world;
    renderer::Camera camera(renderer::Vector3(0, 0, 5), 45.0f,
                            float(width) / float(height), 0.1f);

    renderer.render(world, camera);
    const renderer::FrameBuffer& framebuffer = renderer.getFrameBuffer();

    sf::Image image({width, height});

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            auto color = framebuffer.getPixel(x, y);
            image.setPixel({x, y}, sf::Color(color.x(), color.y(), color.z()));
        }
    }

    image.saveToFile("output.bmp");

    return 0;
}