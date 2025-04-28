#include "AmbientLight.h"

namespace renderer {
AmbientLight::AmbientLight(const Color& intensity)
    : m_intensity(intensity) {
}

AmbientLight& AmbientLight::operator=(const AmbientLight& other) {
    m_intensity = other.m_intensity;
    return *this;
}

void AmbientLight::setIntensity(const Color& intensity) {
    m_intensity = intensity;
}

const Color& AmbientLight::getIntensity() const {
    return m_intensity;
}
} // namespace renderer