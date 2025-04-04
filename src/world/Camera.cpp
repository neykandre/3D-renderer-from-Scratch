#include "Camera.h"

namespace renderer {
Far Camera::getFar() const {
    return m_far;
}

Near Camera::getNear() const {
    return m_near;
}

Fov Camera::getFov() const {
    return m_fov;
}

Matrix4 Camera::getViewMatrix() const {
    return m_viewMatrix;
}
} // namespace renderer