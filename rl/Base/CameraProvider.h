#pragma once

#include "StateDrawable.h"
#include "rl/Base/DeviceInputReceiver.h"
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Rl::Providers {

class AbstractCamera {
public:
    struct Eye
    {
        double x, y, z;
    };
    struct PVMMatrix
    {
        std::array<float, 16> matrix;
    };
    double far, near;
    Eye eye;
    float fov;
    float aspectRatio;
    float zoom;
    float pitch, yaw;
    virtual ~AbstractCamera() = default;
    AbstractCamera() = default;
    AbstractCamera(const AbstractCamera& other) = delete;
    AbstractCamera& operator=(const AbstractCamera& other) = delete;
    virtual void SetPVMMatrix(const PVMMatrix& mvp) = 0;
    virtual void SetRotateXYZ(const Eye& eye) = 0;
    virtual void SetEyePosition(const Eye& eye) = 0;
    virtual void SetFar(double far) = 0;
    virtual void SetNear(double near) = 0;
    virtual void SetAspectRatio(float aspectRatio) = 0;
    virtual void SetFov(float fov) = 0;
    virtual void SetZoom(float zoom) = 0;
    virtual float GetAspectRatio() const = 0;
};

struct CameraInputReceiver : public virtual Input::InputObserver {
    void OnKeyEvent(const Input::KeyEvent& event) override = 0;
    void OnMouseButtonEvent(const Input::MouseButtonEvent& event) override = 0;
    void OnMouseMoveEvent(const Input::MouseMoveEvent& event) override = 0;
    void OnMouseScrollEvent(const Input::MouseScrollEvent& event) override = 0;
};

class Camera : public AbstractCamera, public CameraInputReceiver {
public:
    Camera();
    Camera(Camera& other) = delete;
    Camera& operator=(Camera& other) = delete;
    ~Camera() override;
    void Update();
    void SetPVMMatrix(const PVMMatrix& mvp) override;
    void SetRotateXYZ(const Eye& eye) override;
    void SetEyePosition(const Eye& eye) override;
    void SetFar(double far) override;
    void SetNear(double near) override;
    void SetAspectRatio(float aspectRatio) override;
    void SetFov(float fov) override;
    void SetZoom(float zoom) override;
    float GetAspectRatio() const override;
    void OnKeyEvent(const Input::KeyEvent& event) override;
    void OnMouseButtonEvent(const Input::MouseButtonEvent& event) override;
    void OnMouseMoveEvent(const Input::MouseMoveEvent& event) override;
    void OnMouseScrollEvent(const Input::MouseScrollEvent& event) override;
};

// Just for data interchange between classes
class CameraStateDrawableResources : StateDrawableResources<Camera> {
};

struct CameraStateDrawable : public StateDrawable<CameraStateDrawableResources>
{
    void OnDraw() override;
    void OnUpdate() override;
    void OnCreate(StateDrawableResources<CameraStateDrawableResources>& resources) override;
    void OnDestroy() override;
    void OnPause() override;
    void OnResume() override;
};

} // namespace Rl::Providers
