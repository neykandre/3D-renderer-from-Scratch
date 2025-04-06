#include "Camera.h"

namespace renderer {
const Vector3 kForwardVector  = { 0, 0, -1 };
const Vector3 kRightVector    = { 1, 0, 0 };
const Vector3 kHorizontalAxis = { 0, -1, 0 };
const Vector3 kVerticalAxis   = { -1, 0, 0 };

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

void Camera::moveForward(float distance) {
    Vector3 shift = kForwardVector * distance;
    m_viewMatrix  = makeTranslationMatrix(-shift) * m_viewMatrix;
}

void Camera::moveBack(float distance) {
    moveForward(-distance);
}

void Camera::moveRight(float distance) {
    Vector3 shift = kRightVector * distance;
    m_viewMatrix  = makeTranslationMatrix(-shift) * m_viewMatrix;
}

void Camera::moveLeft(float distance) {
    moveRight(-distance);
}

void Camera::rotateHorizontal(float angle) {
    m_viewMatrix = makeRotationMatrix(kHorizontalAxis, -angle) * m_viewMatrix;
}

void Camera::rotateVertical(float angle) {
    m_viewMatrix = makeRotationMatrix(kVerticalAxis, -angle) * m_viewMatrix;
}

} // namespace renderer