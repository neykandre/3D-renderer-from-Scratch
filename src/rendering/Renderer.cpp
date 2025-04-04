#include "Renderer.h"
#include "Screen.h"

namespace renderer {
Screen Renderer::render(const World& world, const Camera& camera, Screen&& screen) {
    screen.fillWithColor(sf::Color::Black);
    for (const Object& object : world.getObjects()) {
        for (const Triangle& triangle : object.getTriangles()) {

        }
    }
    return screen;
}
} // namespace renderer