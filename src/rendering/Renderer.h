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

    Vertex calcIntersection(const Vertex& v1, const Vertex& v2, float t);

    std::vector<Triangle> TriangulatePolygon(const std::vector<Vertex>& vertices);

    void rasterizeTriangle(const Triangle& triangle,
                           const AmbientLight& ambientLight,
                           const std::vector<DirectionalLight>& directionalLights,
                           Screen& screen);

    Vector3 toNdcTransform(const Vector4& point);

    float calcOrientedArea(const Vector3& p0, const Vector3& p1, const Vector3& p2);
};
} // namespace renderer