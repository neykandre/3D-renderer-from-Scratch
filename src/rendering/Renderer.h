#pragma once

#include "../world/Camera.h"
#include "../world/World.h"
#include "Screen.h"

namespace renderer {
class Renderer {
    using PlaneFunction = std::function<float(const Vector4&)>;

    struct RenderingVertex {
        Vector4 viewPosition;
        Vector4 mvpPosition;
        Vector4 normal;
    };

    using RenderingTriangle = std::array<RenderingVertex, 3>;

  public:
    Screen render(const World& world, const Camera& camera, Screen&& screen);

  private:
    std::vector<Triangle> ClipTriangle(const std::array<Vertex, 3>& vertices);

    std::vector<Vertex> ClipPolygonAgainstPlane(const std::vector<Vertex>& vertices,
                                                const PlaneFunction& plane);

    std::vector<RenderingTriangle>
    ClipRenderingTriangle(const RenderingTriangle& renderingVertices);

    std::vector<RenderingTriangle>
    triangulatePolygon(const std::vector<RenderingVertex>& polygon);

    RenderingVertex calcIntersection(const RenderingVertex& v1,
                                     const RenderingVertex& v2, float t);

    void rasterizeTriangle(const Triangle& triangle,
                           const AmbientLight& ambientLight,
                           const std::vector<DirectionalLight>& directionalLights,
                           Screen& screen);

    void rasterizeTriangleWithPhong(
        const std::array<Vector3, 3>& viewPositions,
        const std::array<Vector3, 3>& screenPositions,
        const std::array<Vector3, 3>& normals, const Material& material,
        const AmbientLight& ambientLight,
        const std::vector<DirectionalLight>& directionalLights, Screen& screen);

    Vector3 toNdcTransform(const Vector4& point);

    Color calcPhongLighting(const AmbientLight& ambientLight,
                            const std::vector<DirectionalLight>& directionalLights,
                            const Vector4& normal, const Material& material);
};
} // namespace renderer
