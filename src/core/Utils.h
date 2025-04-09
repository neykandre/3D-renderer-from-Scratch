#pragma once

#include "../geometry/Triangle.h"
#include "Linalg.h"

namespace renderer {
template <typename Triangle_t, typename Vertex_t>
inline std::vector<Triangle_t>
triangulatePolygon(const std::vector<Vertex_t>& polygon) {
    std::vector<Triangle_t> triangles;
    if (polygon.size() < 3) {
        return triangles;
    }

    for (size_t i = 1; i < polygon.size() - 1; ++i) {
        triangles.push_back(Triangle_t{ polygon[0], polygon[i], polygon[i + 1] });
    }

    return triangles;
}

inline float calcOrientedArea(const Vector3& p0, const Vector3& p1,
                              const Vector3& p2) {
    return (p2.x() - p0.x()) * (p1.y() - p0.y()) -
           (p2.y() - p0.y()) * (p1.x() - p0.x());
}
} // namespace renderer
