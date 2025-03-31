#pragma once

#include "../core/Linalg.h"
#include <vector>

namespace renderer {
enum Width : int;
enum Height : int;

class Screen {
  public:
    Screen(Width width, Height height);

    void fillWithColor(const Vector3& fillColor);
    void setPixel(int x, int y, const Vector3& color);
    Vector3 getPixel(int x, int y) const;

    Width getWidth() const;
    Height getHeight() const;

  private:
    Width m_width;
    Height m_height;
    std::vector<std::uint8_t> m_colorBuffer;
};

} // namespace renderer