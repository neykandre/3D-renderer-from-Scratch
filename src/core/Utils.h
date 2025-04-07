#pragma once

#include "../geometry/Triangle.h"
#include "Linalg.h"

namespace renderer {
inline std::vector<Triangle> triangulatePolygon(const std::vector<Vertex>& polygon) {
    std::vector<Triangle> triangles;
    if (polygon.size() < 3) {
        return triangles;
    }

    for (size_t i = 1; i < polygon.size() - 1; ++i) {
        triangles.emplace_back(polygon[0], polygon[i], polygon[i + 1]);
    }

    return triangles;
}

inline Vertex calcIntersection(const Vertex& v1, const Vertex& v2, float t) {
    return Vertex{
        Vector4{ v1.getPosition() + t * (v2.getPosition() - v1.getPosition()) },
        Vector4{ v1.getNormal() + t * (v2.getNormal() - v1.getNormal()) }
    };
}

inline float calcOrientedArea(const Vector3& p0, const Vector3& p1,
                              const Vector3& p2) {
    return (p2.x() - p0.x()) * (p1.y() - p0.y()) -
           (p2.y() - p0.y()) * (p1.x() - p0.x());
}
} // namespace renderer