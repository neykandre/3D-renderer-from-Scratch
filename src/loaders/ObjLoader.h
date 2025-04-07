#pragma once

#include "../world/Object.h"
#include <optional>

namespace renderer {
class ObjLoader {
  public:
    std::optional<Object> load(const std::string& filename);

  private:
    std::optional<Vector4> parseVertex(std::string_view line);
    std::optional<Vector4> parseNormal(std::string_view line);
    std::vector<std::array<int, 3>> parseFace(std::string_view line);
};
} // namespace renderer
