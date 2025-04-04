#pragma once

#include "../core/Linalg.h"

namespace renderer {
struct Vertex {
    Vector3 position;
    Vector3 normal; // TODO normalize (create constructor)
};
} // namespace renderer