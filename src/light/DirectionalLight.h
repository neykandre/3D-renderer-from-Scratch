#pragma once
#include "../core/Linalg.h"
#include "Color.h"

namespace renderer {
class DirectionalLight {
  public:
    DirectionalLight(const Color& intensity, const Vector4& direction);

    void setIntensity(const Color& intensity);
    void setDirection(const Vector4& direction);

    const Color& getIntensity() const;
    const Vector4& getDirection() const;

  private:
    Color m_intensity;
    Vector4 m_direction;
};
} // namespace renderer
