#include "AmbientLight.h"

namespace renderer {
AmbientLight::AmbientLight(const Vector3& intensity)
    : m_intensity(intensity) {
}

AmbientLight& AmbientLight::operator=(const AmbientLight& other) {
    m_intensity = other.m_intensity;
    return *this;
}

void AmbientLight::setIntensity(const Vector3& intensity) {
    m_intensity = intensity;
}

const Vector3& AmbientLight::getIntensity() const {
    return m_intensity;
}
} // namespace renderer