#include "Application.h"
#include "../loaders/ObjLoader.h"

namespace renderer {
static constexpr float kDefaultMoveSpeed   = 2.5f;
static constexpr float kDefaultSensitivity = 0.001f;

Application::Application(Width width, Height height, const std::string& title)
    : m_window(sf::VideoMode(
                   { static_cast<unsigned>(width), static_cast<unsigned>(height) }),
               title),
      m_texture({ static_cast<unsigned>(width), static_cast<unsigned>(height) }),
      m_sprite(m_texture),
      m_screen(width, height),
      m_moveSpeed(kDefaultMoveSpeed) {

    ObjLoader objLoader;
    auto objOpt = objLoader.load("Lowpoly_tree_sample.obj");
    if (objOpt.has_value()) {
        m_world.addObject(objOpt.value());
    }
}

void Application::run() {
    sf::Clock clock;
    bool isMouseHandling = false;

    const auto onClose = [this](const sf::Event::Closed&) {
        m_window.close();
    };
    const auto onKeyPressed =
        [this, &isMouseHandling](const sf::Event::KeyPressed& keyPressed) {
            if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
                m_window.setMouseCursorVisible(true);
                isMouseHandling = false;
            } else if (keyPressed.scancode == sf::Keyboard::Scancode::LShift) {
                m_moveSpeed *= 5;
            }
        };
    const auto onKeyReleased = [this](const sf::Event::KeyReleased& keyReleased) {
        if (keyReleased.scancode == sf::Keyboard::Scancode::LShift) {
            m_moveSpeed = kDefaultMoveSpeed;
        }
    };
    const auto onMouseButtonPressed =
        [this,
         &isMouseHandling](const sf::Event::MouseButtonPressed& mouseButtonPressed) {
            if (mouseButtonPressed.button == sf::Mouse::Button::Left) {
                m_window.setMouseCursorVisible(false);
                isMouseHandling = true;
            }
        };

    while (m_window.isOpen()) {
        m_window.handleEvents(onClose, onKeyPressed, onKeyReleased,
                              onMouseButtonPressed);

        handleKeyboardInput(clock.restart().asSeconds());
        if (isMouseHandling) {
            handleMouseInput();
        }

        m_window.clear();
        m_screen = m_renderer.render(m_world, m_camera, std::move(m_screen));
        m_texture.update(m_screen.getColorBuffer().data());
        m_sprite.setTexture(m_texture);
        m_window.draw(m_sprite);
        m_window.display();
    }
}

void Application::handleKeyboardInput(float time) {
    float distance = m_moveSpeed * time;
    if (isKeyPressed(sf::Keyboard::Scancode::W)) {
        m_camera.moveForward(distance);
    }
    if (isKeyPressed(sf::Keyboard::Scancode::S)) {
        m_camera.moveBack(distance);
    }
    if (isKeyPressed(sf::Keyboard::Scancode::A)) {
        m_camera.moveLeft(distance);
    }
    if (isKeyPressed(sf::Keyboard::Scancode::D)) {
        m_camera.moveRight(distance);
    }
}

void Application::handleMouseInput() {
    sf::Vector2i mouseCurrentPos = sf::Mouse::getPosition(m_window);
    sf::Vector2i center{ static_cast<int>(m_window.getSize().x / 2),
                         static_cast<int>(m_window.getSize().y / 2) };
    sf::Vector2i delta = mouseCurrentPos - center;

    if (delta.x != 0) {
        m_camera.rotateHorizontal(delta.x * kDefaultSensitivity);
    }
    if (delta.y != 0) {
        m_camera.rotateVertical(delta.y * kDefaultSensitivity);
    }

    sf::Mouse::setPosition(center, m_window);
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