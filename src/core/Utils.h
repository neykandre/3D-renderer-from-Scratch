#pragma once

#include "../geometry/Triangle.h"
#include "Linalg.h"

namespace renderer {
inline float calcOrientedArea(const Vector3& p0, const Vector3& p1,
                              const Vector3& p2) {
    return (p2.x() - p0.x()) * (p1.y() - p0.y()) -
           (p2.y() - p0.y()) * (p1.x() - p0.x());
}
} // namespace renderer
