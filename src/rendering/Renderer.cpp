#include "Renderer.h"
#include "Screen.h"

namespace renderer {
Screen Renderer::render(const World& world, const Camera& camera, Screen&& screen) {
    screen.fillWithColor(sf::Color::Black);

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
        for (const Triangle& triangle : object.getTriangles()) {
            std::array<Vector4, 3> points;
            std::array<Vector4, 3> normals;
            for (int i = 0; i < 3; i++) {
                points[i]  = MVP * triangle.getVertices()[i].getPosition();
                normals[i] = MV * triangle.getVertices()[i].getNormal();
            }

            Vertex v0 = { points[0], normals[0] };
            Vertex v1 = { points[1], normals[1] };
            Vertex v2 = { points[2], normals[2] };
            for (const Triangle& clippedTriangle : ClipTriangle({ v0, v1, v2 })) {
                rasterizeTriangle(clippedTriangle, world.getAmbientLight(),
                                  directionalLights, screen);
            }
        }
    }
    return screen;
}

void Renderer::rasterizeTriangle(
    const Triangle& triangle, const AmbientLight& ambientLight,
    const std::vector<DirectionalLight>& directionalLights, Screen& screen) {
    Vector3 p0 = toNdcTransform(triangle.getVertices()[0].getPosition());
    Vector3 p1 = toNdcTransform(triangle.getVertices()[1].getPosition());
    Vector3 p2 = toNdcTransform(triangle.getVertices()[2].getPosition());

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

            float alpha = calcOrientedArea(p1, p2, pScreen) / area;
            float beta  = calcOrientedArea(p2, p0, pScreen) / area;
            float gamma = calcOrientedArea(p0, p1, pScreen) / area;

            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                float z = alpha * p0.z() + beta * p1.z() + gamma * p2.z();
                if (z < screen.getZBufferElem(x, y)) {
                    if (alpha < 0.01f || beta < 0.01f || gamma < 0.01f) {
                        sf::Color color; // TODO Phong
                        color.r = ambientLight.getIntensity().x() * 255;
                        color.g = ambientLight.getIntensity().y() * 255;
                        color.b = ambientLight.getIntensity().z() * 255;
                        color.a = 255;
                        screen.setPixel(x, y, z, color);
                    } else {
                        screen.setPixel(x, y, z, sf::Color::Black);
                    }
                }
            }
        }
    }
}

Vector3 Renderer::toNdcTransform(const Vector4& point) {
    return Vector3{ point.x() / point.w(), point.y() / point.w(),
                    point.z() / point.w() };
}

float Renderer::calcOrientedArea(const Vector3& p0, const Vector3& p1,
                                 const Vector3& p2) {
    return (p2.x() - p0.x()) * (p1.y() - p0.y()) -
           (p2.y() - p0.y()) * (p1.x() - p0.x());
}

std::vector<Triangle> Renderer::ClipTriangle(const std::array<Vertex, 3>& vertices) {
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

    std::vector<Vertex> polygon = { vertices[0], vertices[1], vertices[2] };
    polygon                     = ClipPolygonAgainstPlane(polygon, planeLeft);
    polygon                     = ClipPolygonAgainstPlane(polygon, planeRight);
    polygon                     = ClipPolygonAgainstPlane(polygon, planeBottom);
    polygon                     = ClipPolygonAgainstPlane(polygon, planeTop);
    polygon                     = ClipPolygonAgainstPlane(polygon, planeNear);
    polygon                     = ClipPolygonAgainstPlane(polygon, planeFar);
    return TriangulatePolygon(polygon);
}

std::vector<Vertex> Renderer::ClipPolygonAgainstPlane(
    const std::vector<Vertex>& vertices, const PlaneFunction& plane) {
    std::vector<Vertex> polygon;
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Vertex& current = vertices[i];
        const Vertex& next    = vertices[(i + 1) % vertices.size()];

        float currentValue = plane(current.getPosition());
        float nextValue    = plane(next.getPosition());

        bool isCurrentInside = (currentValue >= 0);
        bool isNextInside    = (nextValue >= 0);

        if (isCurrentInside && isNextInside) {
            polygon.push_back(next);
        } else if (isCurrentInside) {
            float t             = currentValue / (currentValue - nextValue);
            Vertex intersection = calcIntersection(current, next, t);
            polygon.push_back(intersection);
        } else if (isNextInside) {
            float t             = currentValue / (currentValue - nextValue);
            Vertex intersection = calcIntersection(current, next, t);
            polygon.push_back(intersection);
            polygon.push_back(next);
        }
    }
    return polygon;
}

Vertex Renderer::calcIntersection(const Vertex& v1, const Vertex& v2, float t) {
    return Vertex{
        Vector4{ v1.getPosition() + t * (v2.getPosition() - v1.getPosition()) },
        Vector4{ v1.getNormal() + t * (v2.getNormal() - v1.getNormal()) }
    };
}

std::vector<Triangle> Renderer::TriangulatePolygon(
    const std::vector<Vertex>& polygon) {
    std::vector<Triangle> triangles;
    if (polygon.size() < 3) {
        return triangles;
    }

    for (size_t i = 1; i < polygon.size() - 1; ++i) {
        triangles.emplace_back(polygon[0], polygon[i], polygon[i + 1]);
    }

    return triangles;
}
} // namespace renderer