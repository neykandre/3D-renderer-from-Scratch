#pragma once
#include "../core/Linalg.h"

namespace renderer {
constexpr Vector3 kDefaultAmbientIntensity{ 0.2f, 0.2f, 0.2f };
class AmbientLight {
  public:
    AmbientLight() = default;
    explicit AmbientLight(const Vector3& intensity);

    void setIntensity(const Vector3& intensity);

    const Vector3& getIntensity() const;

  private:
    Vector3 m_intensity = kDefaultAmbientIntensity;
};
} // namespace renderer
