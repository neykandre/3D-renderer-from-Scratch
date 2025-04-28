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
    std::vector<RenderingTriangle>
    clipRenderingTriangle(const RenderingTriangle& triangle);

    std::vector<RenderingVertex>
    clipPolygonAgainstPlane(const std::vector<RenderingVertex>& vertices,
                            const PlaneFunction& plane);

    RenderingVertex calcIntersection(const RenderingVertex& v1,
                                     const RenderingVertex& v2, float t);

    void rasterizeTriangleWithPhong(
        const RenderingTriangle& renderingTriangle, const Material& material,
        const AmbientLight& ambientLight,
        const std::vector<DirectionalLight>& directionalLights, Screen& screen);

    Color
    calcBlinnPhongPixelColor(const Vector3& normal, const Vector3& view,
                             const Material& material,
                             const std::vector<DirectionalLight>& directionalLights,
                             const AmbientLight& ambientLight);

    template <typename T>
    constexpr inline T interpolate(float alpha, float beta, float gamma, const T& t1,
                                   const T& t2, const T& t3) {
        return alpha * t1 + beta * t2 + gamma * t3;
    }

    Vector3 toNdcTransform(const Vector4& point);
};
} // namespace renderer
