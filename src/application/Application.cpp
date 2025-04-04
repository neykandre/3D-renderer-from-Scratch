#include "Application.h"

namespace renderer {
Application::Application(Width width, Height height, const std::string& title)
    : m_window(sf::VideoMode(
                   { static_cast<unsigned>(width), static_cast<unsigned>(height) }),
               title),
      m_texture({ static_cast<unsigned>(width), static_cast<unsigned>(height) }),
      m_sprite(m_texture),
      m_screen(width, height) {

    Vertex v0 = { { -10.0f, -10.0f, 0.5f }, { 0.0f, 0.0f, -1.0f } };
    Vertex v1 = { { 10.0f, -10.0f, 0.5f }, { 0.0f, 0.0f, -1.0f } };
    Vertex v2 = { { 10.0f, 10.0f, 0.5f }, { 0.0f, 0.0f, -1.0f } };

    Object o0;
    o0.addTriangle({ v0, v1, v2 });

    m_world.addObject(o0);
}

void Application::run() {
    while (m_window.isOpen()) {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            }
        }

        m_window.clear();
        m_screen = m_renderer.render(m_world, m_camera, std::move(m_screen));
        m_texture.update(m_screen.getColorBuffer().data());
        m_sprite.setTexture(m_texture);
        m_window.draw(m_sprite);
        m_window.display();
    }
}
} // namespace renderer