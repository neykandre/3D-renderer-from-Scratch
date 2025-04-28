#include "Material.h"

namespace renderer {
Material::Material(const Color& ambient, const Color& diffuse, const Color& specular,
                   float shininess)
    : m_ambient(ambient),
      m_diffuse(diffuse),
      m_specular(specular),
      m_shininess(shininess) {
}

Material& Material::operator=(const Material& other) {
    m_ambient   = other.m_ambient;
    m_diffuse   = other.m_diffuse;
    m_specular  = other.m_specular;
    m_shininess = other.m_shininess;
    return *this;
}

const Color& Material::getAmbient() const {
    return m_ambient;
}

void Material::setAmbient(const Color& ambient) {
    m_ambient = ambient;
}

const Color& Material::getDiffuse() const {
    return m_diffuse;
}

void Material::setDiffuse(const Color& diffuse) {
    m_diffuse = diffuse;
}

const Color& Material::getSpecular() const {
    return m_specular;
}

void Material::setSpecular(const Color& specular) {
    m_specular = specular;
}

float Material::getShininess() const {
    return m_shininess;
}

void Material::setShininess(float shininess) {
    m_shininess = shininess;
}
} // namespace renderer
