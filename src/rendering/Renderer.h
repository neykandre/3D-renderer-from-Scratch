#pragma once

#include "../world/Camera.h"
#include "../world/World.h"
#include "Screen.h"

namespace renderer {
class Renderer {
  public:
    Screen render(const World& world, const Camera& camera, Screen&& screen);

  private:
    void rasterizeTriangle(const std::array<Vector4, 3>& points,
                           const std::array<Vector4, 3>& normals,
                           const AmbientLight& ambientLight,
                           const std::vector<DirectionalLight>& directionalLights,
                           Screen& screen);

    Vector3 toScreenTransform(const Vector4& point);
};
} // namespace renderer