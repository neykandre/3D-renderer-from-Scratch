#include "Screen.h"
#include <cassert>

namespace renderer {

Screen::Screen(unsigned int width, unsigned int height)
    : m_width(width),
      m_height(height),
      m_colorBuffer(width * height, Vector3{ 0, 0, 0 }) {
}

void Screen::clear(const Vector3& clearColor) {
    std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), clearColor);
}

void Screen::setPixel(int x, int y, const Vector3& color) {
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height &&
           "(x, y) is outside bounds of Screen");
    int index            = y * m_width + x;
    m_colorBuffer[index] = color;
}

Vector3 Screen::getPixel(int x, int y) const {
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height &&
           "(x, y) is outside bounds of Screen");
    return m_colorBuffer[y * m_width + x];
}

unsigned int Screen::getWidth() const {
    return m_width;
}

unsigned int Screen::getHeight() const {
    return m_height;
}

} // namespace renderer