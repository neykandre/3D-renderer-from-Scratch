#pragma once

#include "../core/Vector.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace renderer {

class FrameBuffer {
  public:
    FrameBuffer(int width, int height);

    void clear(const Vector3& clearColor);
    void setPixel(int x, int y, const Vector3& color);
    Vector3 getPixel(int x, int y) const;

    int getWidth() const;
    int getHeight() const;

  private:
    unsigned int m_width, m_height;
    std::vector<Vector3> m_colorBuffer;
};
} // namespace renderer