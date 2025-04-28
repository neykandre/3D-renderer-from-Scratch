#include "Renderer.h"
#include "../core/Linalg.h"
#include "../core/Utils.h"
#include "Screen.h"

namespace renderer {
Screen Renderer::render(const World& world, const Camera& camera, Screen&& screen) {
    screen.fillBuffer(0);

    std::vector<DirectionalLight> directionalLights;
    for (const DirectionalLight& light : world.getDirectionalLights()) {
        directionalLights.emplace_back(
            light.getIntensity(), camera.getViewMatrix() * light.getDirection());
    }

    Matrix4 VP =
        makeProjectionMatrix(camera.getNear(), camera.getFar(), camera.getFov(),
                             screen.getWidth() / screen.getHeight()) *
        camera.getViewMatrix();

    for (const Object& object : world.getObjects()) {
        Matrix4 MVP = VP * object.getModelMatrix();
        Matrix4 MV  = camera.getViewMatrix() * object.getModelMatrix();

        size_t totalVertices = object.getTriangles().size() * 3;
        Matrix4Xn viewPositions(4, totalVertices);
        Matrix4Xn mvpPositions(4, totalVertices);
        Matrix4Xn normals(4, totalVertices);

        const auto& triangles = object.getTriangles();
        for (int i = 0; i < triangles.size(); ++i) {
            viewPositions.col(3 * i) = triangles[i].getVertices()[0].getPosition();
            viewPositions.col(3 * i + 1) =
                triangles[i].getVertices()[1].getPosition();
            viewPositions.col(3 * i + 2) =
                triangles[i].getVertices()[2].getPosition();

            mvpPositions.col(3 * i) = triangles[i].getVertices()[0].getPosition();
            mvpPositions.col(3 * i + 1) =
                triangles[i].getVertices()[1].getPosition();
            mvpPositions.col(3 * i + 2) =
                triangles[i].getVertices()[2].getPosition();

            normals.col(3 * i)     = triangles[i].getVertices()[0].getNormal();
            normals.col(3 * i + 1) = triangles[i].getVertices()[1].getNormal();
            normals.col(3 * i + 2) = triangles[i].getVertices()[2].getNormal();
        }

        viewPositions = MV * viewPositions;
        mvpPositions  = MVP * mvpPositions;
        normals       = MV * normals;

        std::vector<RenderingTriangle> clippedTriangles;
        for (int i = 0; i < triangles.size(); ++i) {
            RenderingTriangle triangle;
            for (int j = 0; j < 3; ++j) {
                triangle[j] = { viewPositions.col(3 * i + j),
                                mvpPositions.col(3 * i + j),
                                normals.col(3 * i + j) };
            }
            auto currentClippedTriangles = clipRenderingTriangle(triangle);
            clippedTriangles.insert(clippedTriangles.end(),
                                    currentClippedTriangles.begin(),
                                    currentClippedTriangles.end());
        }

        for (const RenderingTriangle& clippedTriangle : clippedTriangles) {
            rasterizeTriangleWithPhong(clippedTriangle, object.getMaterial(),
                                       world.getAmbientLight(), directionalLights,
                                       screen);
        }
    }
    return screen;
}

void Renderer::rasterizeTriangleWithPhong(
    const RenderingTriangle& renderingTriangle, const Material& material,
    const AmbientLight& ambientLight,
    const std::vector<DirectionalLight>& directionalLights, Screen& screen) {
    Vector3 normal0 = renderingTriangle[0].normal.head<3>();
    Vector3 normal1 = renderingTriangle[1].normal.head<3>();
    Vector3 normal2 = renderingTriangle[2].normal.head<3>();

    Vector3 view0 = renderingTriangle[0].viewPosition.head<3>();
    Vector3 view1 = renderingTriangle[1].viewPosition.head<3>();
    Vector3 view2 = renderingTriangle[2].viewPosition.head<3>();

    Vector3 p0 = toNdcTransform(renderingTriangle[0].mvpPosition);
    Vector3 p1 = toNdcTransform(renderingTriangle[1].mvpPosition);
    Vector3 p2 = toNdcTransform(renderingTriangle[2].mvpPosition);

    // to screen transformation
    p0.x() = (p0.x() + 1.0f) * screen.getWidth() / 2.0f;
    p0.y() = (1 - (p0.y() + 1.0f) / 2.0f) * screen.getHeight();
    p1.x() = (p1.x() + 1.0f) * screen.getWidth() / 2.0f;
    p1.y() = (1 - (p1.y() + 1.0f) / 2.0f) * screen.getHeight();
    p2.x() = (p2.x() + 1.0f) * screen.getWidth() / 2.0f;
    p2.y() = (1 - (p2.y() + 1.0f) / 2.0f) * screen.getHeight();

    float minX = std::min({ p0.x(), p1.x(), p2.x() });
    float maxX = std::max({ p0.x(), p1.x(), p2.x() });
    float minY = std::min({ p0.y(), p1.y(), p2.y() });
    float maxY = std::max({ p0.y(), p1.y(), p2.y() });

    int x0 = std::max(0, static_cast<int>(std::floor(minX)));
    int y0 = std::max(0, static_cast<int>(std::floor(minY)));
    int x1 = std::min(static_cast<int>(screen.getWidth()) - 1,
                      static_cast<int>(std::ceil(maxX)));
    int y1 = std::min(static_cast<int>(screen.getHeight()) - 1,
                      static_cast<int>(std::ceil(maxY)));

    float area = calcOrientedArea(p0, p1, p2);
    if (area == 0) {
        return;
    }

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            Vector3 pScreen = { x + 0.5f, y + 0.5f, 0.0f };
            float alpha     = calcOrientedArea(p1, p2, pScreen) / area;
            float beta      = calcOrientedArea(p2, p0, pScreen) / area;
            float gamma     = calcOrientedArea(p0, p1, pScreen) / area;
            float z = interpolate(alpha, beta, gamma, p0.z(), p1.z(), p2.z());

            if (alpha >= 0 && beta >= 0 && gamma >= 0 &&
                z < screen.getZBufferElem(x, y)) {
                Vector3 interpolatedNormal =
                    interpolate(alpha, beta, gamma, normal0, normal1, normal2)
                        .normalized();
                Vector3 interpolatedView =
                    -interpolate(alpha, beta, gamma, view0, view1, view2)
                         .normalized();
                Color pixelColor = calcBlinnPhongPixelColor(
                    interpolatedNormal, interpolatedView, material,
                    directionalLights, ambientLight);
                screen.setPixel(x, y, z, pixelColor.convertToSfColor());
            }
        }
    }
}

Color Renderer::calcBlinnPhongPixelColor(
    const Vector3& normal, const Vector3& view, const Material& material,
    const std::vector<DirectionalLight>& directionalLights,
    const AmbientLight& ambientLight) {
    Color ambient = material.getAmbient() * ambientLight.getIntensity();

    Color totalDiffuseSpecular;
    for (const auto& light : directionalLights) {
        float diff    = std::max(0.f, normal.dot(-light.getDirection().head<3>()));
        Color diffuse = material.getDiffuse() * light.getIntensity() * diff;

        Vector3 H  = (-light.getDirection().head<3>() + view).normalized();
        float spec = std::pow(std::max(0.f, normal.dot(H)), material.getShininess());
        Color specular = material.getSpecular() * light.getIntensity() * spec;

        totalDiffuseSpecular += diffuse + specular;
    }

    return ambient + totalDiffuseSpecular;
}

Renderer::RenderingVertex Renderer::calcIntersection(const RenderingVertex& v1,
                                                     const RenderingVertex& v2,
                                                     float t) {

    return RenderingVertex{ v1.viewPosition +
                                t * (v2.viewPosition - v1.viewPosition),
                            v1.mvpPosition + t * (v2.mvpPosition - v1.mvpPosition),
                            (v1.normal + t * (v2.normal - v1.normal)).normalized() };
}

Vector3 Renderer::toNdcTransform(const Vector4& point) {
    return Vector3{ point.x() / point.w(), point.y() / point.w(),
                    point.z() / point.w() };
}

std::vector<Renderer::RenderingTriangle>
Renderer::clipRenderingTriangle(const RenderingTriangle& triangle) {
    PlaneFunction planeLeft = [](const Vector4& v) {
        return v.x() + v.w();
    };
    PlaneFunction planeRight = [](const Vector4& v) {
        return -v.x() + v.w();
    };
    PlaneFunction planeBottom = [](const Vector4& v) {
        return v.y() + v.w();
    };
    PlaneFunction planeTop = [](const Vector4& v) {
        return -v.y() + v.w();
    };
    PlaneFunction planeNear = [](const Vector4& v) {
        return v.z() + v.w();
    };
    PlaneFunction planeFar = [](const Vector4& v) {
        return -v.z() + v.w();
    };

    std::vector<RenderingVertex> polygon = { triangle[0], triangle[1], triangle[2] };
    polygon = clipPolygonAgainstPlane(polygon, planeLeft);
    polygon = clipPolygonAgainstPlane(polygon, planeRight);
    polygon = clipPolygonAgainstPlane(polygon, planeBottom);
    polygon = clipPolygonAgainstPlane(polygon, planeTop);
    polygon = clipPolygonAgainstPlane(polygon, planeNear);
    polygon = clipPolygonAgainstPlane(polygon, planeFar);
    return triangulatePolygon<RenderingTriangle, RenderingVertex>(polygon);
}

std::vector<Renderer::RenderingVertex>
Renderer::clipPolygonAgainstPlane(const std::vector<RenderingVertex>& vertices,
                                  const PlaneFunction& plane) {
    std::vector<RenderingVertex> polygon;
    for (size_t i = 0; i < vertices.size(); ++i) {
        const auto& current = vertices[i];
        const auto& next    = vertices[(i + 1) % vertices.size()];

        float currentValue = plane(current.mvpPosition);
        float nextValue    = plane(next.mvpPosition);

        bool isCurrentInside = (currentValue >= 0);
        bool isNextInside    = (nextValue >= 0);

        if (isCurrentInside && isNextInside) {
            polygon.push_back(next);
        } else if (isCurrentInside) {
            float t                      = currentValue / (currentValue - nextValue);
            RenderingVertex intersection = calcIntersection(current, next, t);
            polygon.push_back(intersection);
        } else if (isNextInside) {
            float t                      = currentValue / (currentValue - nextValue);
            RenderingVertex intersection = calcIntersection(current, next, t);
            polygon.push_back(intersection);
            polygon.push_back(next);
        }
    }
    return polygon;
}
} // namespace renderer
