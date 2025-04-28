#include "Color.h"
#include <algorithm>

namespace renderer {
Color::Color(float r, float g, float b, float a)
    : m_r(std::clamp(r, 0.f, 1.f)),
      m_g(std::clamp(g, 0.f, 1.f)),
      m_b(std::clamp(b, 0.f, 1.f)),
      m_a(std::clamp(a, 0.f, 1.f)) {
}

sf::Color Color::convertToSfColor() const {
    return { static_cast<std::uint8_t>(m_r * 255),
             static_cast<std::uint8_t>(m_g * 255),
             static_cast<std::uint8_t>(m_b * 255),
             static_cast<std::uint8_t>(m_a * 255) };
}

Color& Color::operator+=(const Color& other) {
    m_r = std::clamp(m_r + other.m_r, 0.f, 1.f);
    m_g = std::clamp(m_g + other.m_g, 0.f, 1.f);
    m_b = std::clamp(m_b + other.m_b, 0.f, 1.f);
    m_a = std::clamp(m_a + other.m_a, 0.f, 1.f);
    return *this;
}

Color& Color::operator*=(const Color& other) {
    m_r = std::clamp(m_r * other.m_r, 0.f, 1.f);
    m_g = std::clamp(m_g * other.m_g, 0.f, 1.f);
    m_b = std::clamp(m_b * other.m_b, 0.f, 1.f);
    m_a = std::clamp(m_a * other.m_a, 0.f, 1.f);
    return *this;
}

Color& Color::operator*=(float alpha) {
    m_r = std::clamp(m_r * alpha, 0.f, 1.f);
    m_g = std::clamp(m_g * alpha, 0.f, 1.f);
    m_b = std::clamp(m_b * alpha, 0.f, 1.f);
    m_a = std::clamp(m_a * alpha, 0.f, 1.f);
    return *this;
}

Color Color::operator+(const Color& other) const {
    Color newColor = *this;
    return newColor += other;
}

Color Color::operator*(const Color& other) const {
    Color newColor = *this;
    return newColor *= other;
}

Color Color::operator*(float alpha) const {
    Color newColor = *this;
    return newColor *= alpha;
}

Color operator*(float alpha, const Color& color) {
    return color * alpha;
}
} // namespace renderer