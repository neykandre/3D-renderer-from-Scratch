#pragma once

#include "../core/SpscQueue.h"
#include "../rendering/Renderer.h"
#include "../rendering/Screen.h"
#include "../ui/Command.h"
#include "../ui/ConsoleInterface.h"
#include "../world/Camera.h"
#include "../world/World.h"

#include <thread>

namespace renderer {
class Application {
  public:
    Application(Width width, Height height, const std::string& title);
    ~Application();

    void run();

  private:
    void handleEvents(bool& isMouseHandling, float& cameraMoveSpeed);
    void handleKeyboardInput(float time, float cameraMoveSpeed);
    void handleMouseInput();
    void processCommands();

    World m_world;
    Renderer m_renderer;
    Screen m_screen;
    Camera m_camera;
    sf::RenderWindow m_window;
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    SpscQueue<Command, ConsoleInterface::QSIZE> m_queue;
    ConsoleInterface m_console;
    std::thread m_consoleThread;
};
} // namespace renderer
