#pragma once

#include "../world/Camera.h"
#include "../world/World.h"
#include "Screen.h"

namespace renderer {
class Renderer {
    using PlaneFunction = std::function<float(const Vector4&)>;

  public:
    Screen render(const World& world, const Camera& camera, Screen&& screen);

  private:
    std::vector<Triangle> ClipTriangle(const std::array<Vertex, 3>& vertices);

    std::vector<Vertex> ClipPolygonAgainstPlane(const std::vector<Vertex>& vertices,
                                                const PlaneFunction& plane);

    void rasterizeTriangle(const Triangle& triangle,
                           const AmbientLight& ambientLight,
                           const std::vector<DirectionalLight>& directionalLights,
                           Screen& screen);

    Vector3 toNdcTransform(const Vector4& point);
};
} // namespace renderer