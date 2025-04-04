#include "World.h"

namespace renderer {
void World::addObject(Object object) {
    m_objects.emplace_back(std::move(object));
}

void World::addDirectionalLight(DirectionalLight light) {
    m_directionalLights.emplace_back(std::move(light));
}

void World::setAmbientLight(const AmbientLight& light) {
    m_ambientLight = light;
}

const std::vector<DirectionalLight>& World::getDirectionalLights() const {
    return m_directionalLights;
}

const AmbientLight& World::getAmbientLight() const {
    return m_ambientLight;
}

const std::vector<Object>& World::getObjects() const {
    return m_objects;
}
} // namespace renderer