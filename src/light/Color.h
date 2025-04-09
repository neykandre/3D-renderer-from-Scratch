#pragma once
#include "SFML/Graphics/Color.hpp"

namespace renderer {
class Color {
  public:
    Color(float r, float g, float b, float a = 1.f);

    sf::Color convertToSfColor() const;

    Color& operator+=(const Color& other);
    Color& operator*=(const Color& other);
    Color& operator*=(float alpha);
    Color operator+(const Color& other) const;
    Color operator*(const Color& other) const;
    Color operator*(float alpha) const;

  private:
    float m_r;
    float m_g;
    float m_b;
    float m_a;
};
} // namespace renderer
