#include "Application.h"
#include "../loaders/ObjLoader.h"

#include <iostream>

namespace renderer {
static constexpr float kDefaultMoveSpeed   = 2.5f;
static constexpr float kDefaultSensitivity = 0.001f;

Application::Application(Width width, Height height, const std::string& title)
    : m_screen(width, height),
      m_window(sf::VideoMode(
                   { static_cast<unsigned>(width), static_cast<unsigned>(height) }),
               title),
      m_texture({ static_cast<unsigned>(width), static_cast<unsigned>(height) }),
      m_sprite(m_texture),
      m_console(m_queue) {

    m_consoleThread = std::thread(&ConsoleInterface::run, &m_console);
}

Application::~Application() {
    if (m_consoleThread.joinable()) {
        m_consoleThread.join();
    }
}

void Application::processCommands() {
    Command cmd;
    while (m_queue.dequeue(cmd)) {
        switch (cmd.type) {
        case CmdType::LoadObject: {
            ObjLoader L;
            if (auto o = L.load(cmd.path)) {
                auto obj = *o;
                if (cmd.applyTransform) {
                    Vector3 rotVector   = { cmd.rotAxis.x, cmd.rotAxis.y,
                                            cmd.rotAxis.z };
                    Vector3 transVector = { cmd.translation.x, cmd.translation.y,
                                            cmd.translation.z };
                    obj.setModelMatrix(makeRotationMatrix(rotVector, cmd.rotAngle) *
                                       makeTranslationMatrix(transVector));
                }
                m_world.addObject(obj);
                std::cout << "Object was loaded successfully." << std::endl;

            } else {
                std::cerr << "File was not opened." << std::endl;
            }
            break;
        }
        case CmdType::AddLight: {
            Color intensity = { cmd.lightColor.x, cmd.lightColor.y,
                                cmd.lightColor.z };
            m_world.addDirectionalLight(
                { intensity,
                  { cmd.lightDir.x, cmd.lightDir.y, cmd.lightDir.z, 0 } });
            std::cout << "Light was added successfully." << std::endl;
            break;
        }
        case CmdType::Exit: {
            m_window.close();
            break;
        }
        }
    }
}

void Application::run() {
    sf::Clock clock;
    bool isMouseHandling  = false;
    float cameraMoveSpeed = kDefaultMoveSpeed;

    while (m_window.isOpen()) {
        handleEvents(isMouseHandling, cameraMoveSpeed);
        processCommands();

        float dt = clock.restart().asSeconds();
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
    sf::Mouse::setPosition(center, m_window);
    if (delta.x != 0) {
        m_camera.rotateHorizontal(delta.x * kDefaultSensitivity);
    }
    if (delta.y != 0) {
        m_camera.rotateVertical(delta.y * kDefaultSensitivity);
    }
}

} // namespace renderer
