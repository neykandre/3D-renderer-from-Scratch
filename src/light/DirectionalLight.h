#pragma once
#include "../core/Linalg.h"

namespace renderer {
class DirectionalLight {
  public:
    DirectionalLight(const Vector3& intensity, const Vector4& direction);

    void setIntensity(const Vector3& intensity);
    void setDirection(const Vector4& direction);

    const Vector3& getIntensity() const;
    const Vector4& getDirection() const;

  private:
    Vector3 m_intensity;
    Vector4 m_direction;
};
} // namespace renderer
