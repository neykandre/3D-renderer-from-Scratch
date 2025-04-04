#pragma once

#include "../light/AmbientLight.h"
#include "../light/DirectionalLight.h"
#include "Object.h"

namespace renderer {
class World {
  public:
    World() = default;

    void addObject(Object object);
    void addDirectionalLight(DirectionalLight light);
    void setAmbientLight(AmbientLight light);

    const std::vector<Object>& getObjects() const;
    const std::vector<DirectionalLight>& getDirectionalLights() const;
    const AmbientLight& getAmbientLight() const;

  private:
    std::vector<Object> m_objects;
    std::vector<DirectionalLight> m_directionalLights;
    AmbientLight m_ambientLight;
};
} // namespace renderer