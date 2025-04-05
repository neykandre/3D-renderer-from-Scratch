#include "Application.h"

namespace renderer {
Application::Application(Width width, Height height, const std::string& title)
    : m_window(sf::VideoMode(
                   { static_cast<unsigned>(width), static_cast<unsigned>(height) }),
               title),
      m_texture({ static_cast<unsigned>(width), static_cast<unsigned>(height) }),
      m_sprite(m_texture),
      m_screen(width, height) {
    auto cube = CreateCube();
    // cube.setModelMatrix(rotate({ 0, 1, 0 }, 0.2f));
    cube.setModelMatrix(rotate({ 1, 0, 0 }, 0.5f));
    cube.setModelMatrix(rotate({ 0, 1, 0 }, 0.2f) * cube.getModelMatrix());
    cube.setModelMatrix(translate({ -0.5, -1, 0 }) * cube.getModelMatrix());

    m_world.addObject(std::move(cube));
}

void Application::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            }
        }

        update(clock.restart().asSeconds());

        m_window.clear();
        m_screen = m_renderer.render(m_world, m_camera, std::move(m_screen));
        m_texture.update(m_screen.getColorBuffer().data());
        m_sprite.setTexture(m_texture);
        m_window.draw(m_sprite);
        m_window.display();
    }
}

void Application::update(float time) {
    Vector3 moveDir = { 0, 0, 0 };
    Vector3 forward = { 0, 0, 1 };
    Vector3 right   = { -1, 0, 0 };

    if (isKeyPressed(sf::Keyboard::Key::W)) {
        moveDir += forward;
    }
    if (isKeyPressed(sf::Keyboard::Key::S)) {
        moveDir -= forward;
    }
    if (isKeyPressed(sf::Keyboard::Key::A)) {
        moveDir -= right;
    }
    if (isKeyPressed(sf::Keyboard::Key::D)) {
        moveDir += right;
    }

    if (moveDir.norm() > 1e-6f) {
        moveDir.normalize();
        m_camera.move(moveDir);
    }
}

Object Application::CreateCube() {
    Object cube;

    // Определяем границы куба
    float left   = -0.5f;
    float right  = 0.5f;
    float top    = 0.3f;
    float bottom = -0.7f;
    float front  = -3.0f; // ближняя грань (передняя)
    float back   = -5.0f; // дальняя грань (задняя)

    // FRONT FACE (лицевая, нормаль: (0, 0, 1))
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(left, bottom, front), Eigen::Vector3f(0, 0, 1)),
        Vertex(Eigen::Vector3f(right, bottom, front), Eigen::Vector3f(0, 0, 1)),
        Vertex(Eigen::Vector3f(right, top, front), Eigen::Vector3f(0, 0, 1))));
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(right, top, front), Eigen::Vector3f(0, 0, 1)),
        Vertex(Eigen::Vector3f(left, top, front), Eigen::Vector3f(0, 0, 1)),
        Vertex(Eigen::Vector3f(left, bottom, front), Eigen::Vector3f(0, 0, 1))));

    // BACK FACE (обратная, нормаль: (0, 0, -1))
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(right, bottom, back), Eigen::Vector3f(0, 0, -1)),
        Vertex(Eigen::Vector3f(left, bottom, back), Eigen::Vector3f(0, 0, -1)),
        Vertex(Eigen::Vector3f(left, top, back), Eigen::Vector3f(0, 0, -1))));
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(left, top, back), Eigen::Vector3f(0, 0, -1)),
        Vertex(Eigen::Vector3f(right, top, back), Eigen::Vector3f(0, 0, -1)),
        Vertex(Eigen::Vector3f(right, bottom, back), Eigen::Vector3f(0, 0, -1))));

    // LEFT FACE (нормаль: (-1, 0, 0))
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(left, bottom, back), Eigen::Vector3f(-1, 0, 0)),
        Vertex(Eigen::Vector3f(left, bottom, front), Eigen::Vector3f(-1, 0, 0)),
        Vertex(Eigen::Vector3f(left, top, front), Eigen::Vector3f(-1, 0, 0))));
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(left, top, front), Eigen::Vector3f(-1, 0, 0)),
        Vertex(Eigen::Vector3f(left, top, back), Eigen::Vector3f(-1, 0, 0)),
        Vertex(Eigen::Vector3f(left, bottom, back), Eigen::Vector3f(-1, 0, 0))));

    // RIGHT FACE (нормаль: (1, 0, 0))
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(right, bottom, front), Eigen::Vector3f(1, 0, 0)),
        Vertex(Eigen::Vector3f(right, bottom, back), Eigen::Vector3f(1, 0, 0)),
        Vertex(Eigen::Vector3f(right, top, back), Eigen::Vector3f(1, 0, 0))));
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(right, top, back), Eigen::Vector3f(1, 0, 0)),
        Vertex(Eigen::Vector3f(right, top, front), Eigen::Vector3f(1, 0, 0)),
        Vertex(Eigen::Vector3f(right, bottom, front), Eigen::Vector3f(1, 0, 0))));

    // TOP FACE (нормаль: (0, 1, 0))
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(left, top, front), Eigen::Vector3f(0, 1, 0)),
        Vertex(Eigen::Vector3f(right, top, front), Eigen::Vector3f(0, 1, 0)),
        Vertex(Eigen::Vector3f(right, top, back), Eigen::Vector3f(0, 1, 0))));
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(right, top, back), Eigen::Vector3f(0, 1, 0)),
        Vertex(Eigen::Vector3f(left, top, back), Eigen::Vector3f(0, 1, 0)),
        Vertex(Eigen::Vector3f(left, top, front), Eigen::Vector3f(0, 1, 0))));

    // BOTTOM FACE (нормаль: (0, -1, 0))
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(left, bottom, back), Eigen::Vector3f(0, -1, 0)),
        Vertex(Eigen::Vector3f(right, bottom, back), Eigen::Vector3f(0, -1, 0)),
        Vertex(Eigen::Vector3f(right, bottom, front), Eigen::Vector3f(0, -1, 0))));
    cube.addTriangle(Triangle(
        Vertex(Eigen::Vector3f(right, bottom, front), Eigen::Vector3f(0, -1, 0)),
        Vertex(Eigen::Vector3f(left, bottom, front), Eigen::Vector3f(0, -1, 0)),
        Vertex(Eigen::Vector3f(left, bottom, back), Eigen::Vector3f(0, -1, 0))));

    return cube;
}
} // namespace renderer