#pragma once

#include "../core/Linalg.h"
#include "Vertex.h"
#include <array>

namespace renderer {
class Triangle {
  public:
    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

    const std::array<Vertex, 3>& getVertices() const;

  private:
    std::array<Vertex, 3> m_vertices;
};
} // namespace renderer