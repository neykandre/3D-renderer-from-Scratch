#pragma once

#include "../core/Linalg.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace renderer {
enum Width : int;
enum Height : int;

class Screen {
  public:
    Screen(Width width, Height height);

    void fillBuffer(std::uint8_t fillValue);
    void setPixel(int x, int y, float depth, const sf::Color& color);

    Width getWidth() const;
    Height getHeight() const;

    const std::vector<std::uint8_t>& getColorBuffer() const;
    inline float& getZBufferElem(int x, int y) {
        return m_zBuffer[y * m_width + x];
    };

  private:
    Width m_width;
    Height m_height;
    std::vector<std::uint8_t> m_colorBuffer;
    std::vector<float> m_zBuffer;

    inline std::uint8_t& getColorBufferElem(int x, int y, int i) {
        return m_colorBuffer[(y * m_width + x) * 4 + i];
    };
};
} // namespace renderer
