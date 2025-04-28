#include "DirectionalLight.h"

namespace renderer {
DirectionalLight::DirectionalLight(const Color& intensity,
                                   const Vector4& direction)
    : m_intensity(intensity) {
    setDirection(direction);
}

void DirectionalLight::setIntensity(const Color& intensity) {
    m_intensity = intensity;
}

void DirectionalLight::setDirection(const Vector4& direction) {
    m_direction = direction.normalized();
}

const Color& DirectionalLight::getIntensity() const {
    return m_intensity;
}

const Vector4& DirectionalLight::getDirection() const {
    return m_direction;
}
} // namespace renderer