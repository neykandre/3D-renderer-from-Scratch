#pragma once

#include "../rendering/Renderer.h"
#include "../rendering/Screen.h"
#include "../world/Camera.h"
#include "../world/World.h"

namespace renderer {
class Application {
  public:
    Application(Width width, Height height, const std::string& title);

    void run();

  private:
    void handleEvents(bool& isMouseHandling, float& cameraMoveSpeed);
    void handleKeyboardInput(float time, float cameraMoveSpeed);
    void handleMouseInput();
    void showFps(float time, int& frameCount, float& timeAccumulator,
                 sf::Text& fpsText);

    World m_world;
    Renderer m_renderer;
    Screen m_screen;
    Camera m_camera;
    sf::RenderWindow m_window;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};
} // namespace renderer
