#include "Renderer.h"
#include <cassert>

namespace renderer {

Renderer::Renderer(unsigned int width, unsigned int height)
    : m_width(width),
      m_height(height),
      m_framebuffer(width, height) {
    clear();
}

void Renderer::clear() {
    m_framebuffer.clear(Vector3{ 0, 0, 0 });
}

void Renderer::sortVertices(Vector4& v0, Vector4& v1, Vector4& v2) const {
    if (v0.y() > v1.y()) {
        std::swap(v0, v1);
    }
    if (v0.y() > v2.y()) {
        std::swap(v0, v2);
    }
    if (v1.y() > v2.y()) {
        std::swap(v1, v2);
    }
}

void Renderer::rasterizeTriangle(const Triangle& triangle) {
    auto v0 = triangle.getVertices()[0];
    auto v1 = triangle.getVertices()[1];
    auto v2 = triangle.getVertices()[2];

    assert(v0.w() == 1 && v1.w() == 1 && v2.w() == 1 && "w-coordinates must be 1");

    sortVertices(v0, v1, v2);

    float invSlope1 = (v1.x() - v0.x()) / (v1.y() - v0.y());
    float invSlope2 = (v2.x() - v0.x()) / (v2.y() - v0.y());
    float xStart    = v0.x();
    float xEnd      = v0.x();

    for (int y = v0.y(); y <= v1.y(); ++y) {
        for (int x = xStart; x <= xEnd; ++x) {
            m_framebuffer.setPixel(x, y, Vector3{ 255, 255, 255 });
        }
        xStart += invSlope1;
        xEnd += invSlope2;
    }

    invSlope1 = (v2.x() - v1.x()) / (v2.y() - v1.y());
    xStart    = v1.x();

    for (int y = v1.y(); y <= v2.y(); ++y) {
        for (int x = xStart; x <= xEnd; ++x) {
            m_framebuffer.setPixel(x, y, Vector3{ 255, 255, 255 });
        }
        xStart += invSlope1;
        xEnd += invSlope2;
    }
}

bool Renderer::isInsideClipSpace(const Vector4& v) const {
    assert(v.w() == 1 && "w-coordinate must be 1");
    return v.x() >= -1 && v.x() <= 1 && v.y() >= -1 && v.y() <= 1 && v.z() >= -1 &&
           v.z() <= 1;
}

std::vector<Triangle> Renderer::clipTriangle(const Triangle& triangle) const {
    assert(triangle.getVertices()[0].w() == 1 &&
           triangle.getVertices()[1].w() == 1 &&
           triangle.getVertices()[2].w() == 1 && "w-coordinates must be 1");

    std::vector<Triangle> clippedTriangles;

    std::vector<Vector4> vertices(triangle.getVertices().begin(),
                                  triangle.getVertices().end());
    std::vector<Vector4> clippedVertices;

    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t next        = (i + 1) % vertices.size();
        Vector4 current    = vertices[i];
        Vector4 nextVertex = vertices[next];

        bool insideCurrent = isInsideClipSpace(current);
        bool insideNext    = isInsideClipSpace(nextVertex);

        if (insideCurrent) {
            clippedVertices.push_back(current);
        }

        if (insideCurrent != insideNext) {
            float t              = (-current.z()) / (nextVertex.z() - current.z());
            Vector4 intersection = current + t * (nextVertex - current);
            clippedVertices.push_back(intersection);
        }
    }

    if (clippedVertices.size() >= 3) {
        for (size_t i = 1; i < clippedVertices.size() - 1; ++i) {
            Triangle newTriangle(clippedVertices[0], clippedVertices[i],
                                 clippedVertices[i + 1], triangle.getNormal());
            clippedTriangles.push_back(newTriangle);
        }
    }

    return clippedTriangles;
}

void Renderer::render(World& world, const Camera& camera) {
    clear();

    Matrix4 viewMatrix       = camera.getViewMatrix();
    Matrix4 projectionMatrix = camera.getProjectionMatrix();
    Matrix4 finalMatrix      = projectionMatrix * viewMatrix;

    for (auto&& object : world.getObjects()) {
        Matrix4 startTransform = object->getTransform();
        object->applyTransform(finalMatrix);

        Mesh transformedMesh = object->getTransformedMesh();
        for (auto&& triangle : transformedMesh.getTriangles()) {
            if (!isInsideClipSpace(triangle.getVertices()[0]) ||
                !isInsideClipSpace(triangle.getVertices()[1]) ||
                !isInsideClipSpace(triangle.getVertices()[2])) {
                auto clippedTriangles = clipTriangle(triangle);
                for (auto&& clippedTriangle : clippedTriangles) {
                    rasterizeTriangle(clippedTriangle);
                }
            } else {
                rasterizeTriangle(triangle);
            }
        }
    }
}

const FrameBuffer& Renderer::getFrameBuffer() const {
    return m_framebuffer;
}

} // namespace renderer