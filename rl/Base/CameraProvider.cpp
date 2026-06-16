#include "rl/Base/CameraProvider.h"
#include <algorithm>
#include <cmath>

namespace Rl::Providers {

Camera::Camera()
{
    eye.x = 0.0;
    eye.y = 0.0;
    eye.z = 3.0;
    far = 1000.0, near = 0.1, fov = 45.0f;
    aspectRatio = 16.0f / 9.0f;
    zoom = 1.0f, pitch = 0.0f;
    yaw = 0.0f;
}

Camera::~Camera() {}

void Camera::Update()
{

}

void Camera::SetPVMMatrix(const PVMMatrix& mvp)
{
    // Store the PVM matrix
    // In a real implementation, this would be used for rendering
}

void Camera::SetRotateXYZ(const Eye& rotation)
{
    pitch = static_cast<float>(rotation.x);
    yaw = static_cast<float>(rotation.y);
    // rotation.z could be used for roll if needed
}

void Camera::SetEyePosition(const Eye& position)
{
    eye.x = position.x;
    eye.y = position.y;
    eye.z = position.z;
}

void Camera::SetFar(double far) { this->far = far; }

void Camera::SetNear(double near) { this->near = near; }

void Camera::SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }

void Camera::SetFov(float fov) { this->fov = fov; }

void Camera::SetZoom(float zoom) { this->zoom = zoom; }

float Camera::GetAspectRatio() const { return aspectRatio; }

void Camera::OnKeyEvent(const Input::KeyEvent& event)
{
    const float moveSpeed = 0.1f;

    if (event.action == Input::Action::Press || event.action == Input::Action::Repeat)
    {
        switch (event.key)
        {
        case Input::Key::W:
            eye.z -= moveSpeed;
            break;
        case Input::Key::S:
            eye.z += moveSpeed;
            break;
        case Input::Key::A:
            eye.x -= moveSpeed;
            break;
        case Input::Key::D:
            eye.x += moveSpeed;
            break;
        case Input::Key::Space:
            eye.y += moveSpeed;
            break;
        case Input::Key::LeftShift:
            eye.y -= moveSpeed;
            break;
        default:
            break;
        }
    }
}

void Camera::OnMouseButtonEvent(const Input::MouseButtonEvent& event)
{
    // Handle mouse button input for camera control
    if (event.action == Input::Action::Press)
    {
        // Could start mouse look mode
    }
    else if (event.action == Input::Action::Release)
    {
        // Could end mouse look mode
    }
}

void Camera::OnMouseMoveEvent(const Input::MouseMoveEvent& event)
{
    // Handle mouse movement for camera rotation
    static double lastX = 0.0;
    static double lastY = 0.0;
    static bool firstMouse = true;
    if (firstMouse)
    {
        lastX = event.x;
        lastY = event.y;
        firstMouse = false;
        return;
    }
    const double xOffset = event.x - lastX;
    const double yOffset = lastY - event.y; // Reversed since y-coordinates go from bottom to top
    lastX = event.x;
    lastY = event.y;
    constexpr float sensitivity = 0.1f;
    yaw += static_cast<float>(xOffset) * sensitivity;
    pitch += static_cast<float>(yOffset) * sensitivity;
    pitch = std::clamp(pitch, -89.0f, 89.0f);
}

void Camera::OnMouseScrollEvent(const Input::MouseScrollEvent& event)
{
    // Handle mouse scroll for zoom
    zoom -= static_cast<float>(event.yoffset);
    zoom = std::clamp(zoom, 0.1f, 10.0f);
}

} // namespace Rl::Providers
