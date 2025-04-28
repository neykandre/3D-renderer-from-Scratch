#pragma once
#include "../core/Linalg.h"
#include "Color.h"

namespace renderer {
class AmbientLight {
    const Color kDefaultAmbientIntensity{ 0.2f, 0.2f, 0.2f };

  public:
    AmbientLight() = default;
    explicit AmbientLight(const Color& intensity);
    AmbientLight& operator=(const AmbientLight& other);

    void setIntensity(const Color& intensity);

    const Color& getIntensity() const;

  private:
    Color m_intensity = kDefaultAmbientIntensity;
};
} // namespace renderer
