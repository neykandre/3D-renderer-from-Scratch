#include "FrameBuffer.h"
#include <cassert>

namespace renderer {

FrameBuffer::FrameBuffer(int width, int height)
    : m_width(width),
      m_height(height),
      m_colorBuffer(width * height, Vector3{ 0, 0, 0 }) {
}

void FrameBuffer::clear(const Vector3& clearColor) {
    std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), clearColor);
}

void FrameBuffer::setPixel(int x, int y, const Vector3& color) {
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height &&
           "(x, y) is outside bounds of framebuffer");
    int index            = y * m_width + x;
    m_colorBuffer[index] = color;
}

Vector3 FrameBuffer::getPixel(int x, int y) const {
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height &&
           "(x, y) is outside bounds of framebuffer");
    return m_colorBuffer[y * m_width + x];
}

int FrameBuffer::getWidth() const {
    return m_width;
}

int FrameBuffer::getHeight() const {
    return m_height;
}

} // namespace renderer