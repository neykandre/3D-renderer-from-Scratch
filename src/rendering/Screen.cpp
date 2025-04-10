#include "Screen.h"

namespace renderer {
Screen::Screen(Width width, Height height)
    : m_width(width),
      m_height(height),
      m_colorBuffer(width * height * 4),
      m_zBuffer(width * height) {
}

void Screen::fillBuffer(std::uint8_t fillValue) {
    std::ranges::fill(m_colorBuffer, fillValue);
    std::ranges::fill(m_zBuffer, 1.0f);
}

void Screen::setPixel(int x, int y, float depth, const sf::Color& color) {
    getColorBufferElem(x, y, 0) = color.r;
    getColorBufferElem(x, y, 1) = color.g;
    getColorBufferElem(x, y, 2) = color.b;
    getColorBufferElem(x, y, 3) = color.a;
    getZBufferElem(x, y)        = depth;
}

const std::vector<std::uint8_t>& Screen::getColorBuffer() const {
    return m_colorBuffer;
}

Width Screen::getWidth() const {
    return m_width;
}

Height Screen::getHeight() const {
    return m_height;
}
} // namespace renderer
