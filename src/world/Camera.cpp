#include "Camera.h"

namespace renderer {
const Vector3 kForwardVector = { 0, 0, -1 };
const Vector3 kRightVector   = { 1, 0, 0 };
const Vector3 kVerticalAxis  = { -1, 0, 0 };

Far Camera::getFar() const {
    return m_far;
}

Near Camera::getNear() const {
    return m_near;
}

Fov Camera::getFov() const {
    return m_fov;
}

const Matrix4& Camera::getViewMatrix() const {
    return m_viewMatrix;
}

Vector3 Camera::getGlobalUpVector() const {
    return m_viewMatrix.block<3, 1>(0, 1);
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

void Camera::moveUp(float distance) {
    Vector3 shift = getGlobalUpVector() * distance;
    m_viewMatrix  = makeTranslationMatrix(-shift) * m_viewMatrix;
}

void Camera::moveDown(float distance) {
    moveUp(-distance);
}

void Camera::rotateHorizontal(float angle) {
    m_viewMatrix = makeRotationMatrix(getGlobalUpVector(), angle) * m_viewMatrix;
}

void Camera::rotateVertical(float angle) {
    m_viewMatrix = makeRotationMatrix(kVerticalAxis, -angle) * m_viewMatrix;
}

} // namespace renderer