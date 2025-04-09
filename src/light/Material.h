#pragma once
#include "Color.h"

namespace renderer {
class Material {
    const Color kDefaultAmbient   = { 0.2f, 0.2f, 0.2f };
    const Color kDefaultDiffuse   = { 0.8f, 0.8f, 0.8f };
    const Color kDefaultSpecular  = { 1.f, 1.f, 1.f };
    const float kDefaultShininess = 32.0f;

  public:
    Material() = default;
    Material(const Color& ambient, const Color& diffuse, const Color& specular,
             float shininess);
    Material& operator=(const Material& other);

    const Color& getAmbient() const;
    const Color& getDiffuse() const;
    const Color& getSpecular() const;
    float getShininess() const;

    void setAmbient(const Color& ambient);
    void setDiffuse(const Color& diffuse);
    void setSpecular(const Color& specular);
    void setShininess(float shininess);

  private:
    Color m_ambient   = kDefaultAmbient;
    Color m_diffuse   = kDefaultDiffuse;
    Color m_specular  = kDefaultSpecular;
    float m_shininess = kDefaultShininess;
};
} // namespace renderer
