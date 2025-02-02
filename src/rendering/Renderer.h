#pragma once

#include "../world/Camera.h"
#include "../world/World.h"
#include "FrameBuffer.h"

namespace renderer {

class Renderer {
  public:
    Renderer(unsigned int width, unsigned int height);

    void render(World&, const Camera&);
    void clear();

    const FrameBuffer& getFrameBuffer() const;

  private:
    void rasterizeTriangle(const Triangle&);
    void sortVertices(Vector4& v0, Vector4& v1, Vector4& v2) const;

    bool isInsideClipSpace(const Vector4&) const;
    std::vector<Triangle> clipTriangle(const Triangle&) const;

    FrameBuffer m_framebuffer;
    unsigned int m_width;
    unsigned int m_height;
};

} // namespace renderer