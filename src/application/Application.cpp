#include "Application.h"
#include "../loaders/ObjLoader.h"

namespace renderer {
static constexpr float kDefaultMoveSpeed   = 2.5f;
static constexpr float kDefaultSensitivity = 0.001f;
static constexpr std::string kDataPath     = "../../data/";

Application::Application(Width width, Height height, const std::string& title)
    : m_screen(width, height),
      m_window(sf::VideoMode(
                   { static_cast<unsigned>(width), static_cast<unsigned>(height) }),
               title),
      m_texture({ static_cast<unsigned>(width), static_cast<unsigned>(height) }),
      m_sprite(m_texture) {

    ObjLoader objLoader;
    auto objOpt = objLoader.load(kDataPath + "objs/12140_Skull_v3_L2.obj");
    if (objOpt.has_value()) {
        auto obj = objOpt.value();
        obj.setModelMatrix(makeRotationMatrix({ -1, 0, 0 }, 1.5f));
        m_world.addObject(obj);
    }

    m_world.addDirectionalLight(
        { Color{ .7f, .7f, .7f }, Vector4{ -1, -1, -1, 0 } });
}

void Application::run() {
    sf::Clock clock;
    bool isMouseHandling  = false;
    float cameraMoveSpeed = kDefaultMoveSpeed;
    int frameCount        = 0;
    float timeAccumulator = 0.f;
    sf::Font font;
    sf::Text fpsText(font);

    while (m_window.isOpen()) {
        handleEvents(isMouseHandling, cameraMoveSpeed);

        float dt = clock.restart().asSeconds();
        showFps(dt, frameCount, timeAccumulator, fpsText);
        handleKeyboardInput(dt, cameraMoveSpeed);
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

void Application::handleEvents(bool& isMouseHandling, float& cameraMoveSpeed) {
    static const auto onClose = [this](const sf::Event::Closed&) {
        m_window.close();
    };
    static const auto onKeyPressed = [this, &isMouseHandling, &cameraMoveSpeed](
                                         const sf::Event::KeyPressed& keyPressed) {
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
            m_window.setMouseCursorVisible(true);
            isMouseHandling = false;
        } else if (keyPressed.scancode == sf::Keyboard::Scancode::LShift) {
            cameraMoveSpeed *= 5;
        }
    };
    static const auto onKeyReleased =
        [this, &cameraMoveSpeed](const sf::Event::KeyReleased& keyReleased) {
            if (keyReleased.scancode == sf::Keyboard::Scancode::LShift) {
                cameraMoveSpeed = kDefaultMoveSpeed;
            }
        };
    static const auto onMouseButtonPressed =
        [this,
         &isMouseHandling](const sf::Event::MouseButtonPressed& mouseButtonPressed) {
            if (mouseButtonPressed.button == sf::Mouse::Button::Left) {
                m_window.setMouseCursorVisible(false);
                isMouseHandling = true;
            }
        };

    m_window.handleEvents(onClose, onKeyPressed, onKeyReleased,
                          onMouseButtonPressed);
}

void Application::handleKeyboardInput(float time, float cameraMoveSpeed) {
    float distance = cameraMoveSpeed * time;
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
    if (isKeyPressed(sf::Keyboard::Scancode::Space)) {
        m_camera.moveUp(distance);
    }
    if (isKeyPressed(sf::Keyboard::Scancode::LControl)) {
        m_camera.moveDown(distance);
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

void Application::showFps(float time, int& frameCount, float& timeAccumulator,
                          sf::Text& fpsText) {
    frameCount++;
    timeAccumulator += time;
    if (timeAccumulator >= 0.5f) {
        float fps       = frameCount / timeAccumulator;
        frameCount      = 0;
        timeAccumulator = 0.0f;
        std::ostringstream oss;
        oss << "FPS: " << static_cast<int>(fps);
        fpsText.setString(oss.str());
    }
}

} // namespace renderer
