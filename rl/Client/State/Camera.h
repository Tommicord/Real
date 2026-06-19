#pragma once

#include "rl/Base/Game.h"
#include "rl/Base/InputReceiver.h"
#include "rl/Base/StateDrawable.h"

#include <array>
#include <vulkan/vulkan.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rl/Base/IUpdatable.h"
#include "rl/Base/StateModel.h"

namespace Rl::Providers {
class AbstractCamera
{
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
    virtual void SetPVMMatrix(const PVMMatrix& mvp) = 0;
    virtual void SetRotateXYZ(const Eye& eye) = 0;
    virtual void SetEyePosition(const Eye& eye) = 0;
    virtual void SetFar(double far) = 0;
    virtual void SetNear(double near) = 0;
    virtual void SetAspectRatio(float aspectRatio) = 0;
    virtual void SetFov(float fov) = 0;
    virtual void SetZoom(float zoom) = 0;
    [[nodiscard]]
    virtual float GetAspectRatio() const = 0;
    [[nodiscard]]
    virtual glm::mat4 GetViewMatrix() const = 0;
    [[nodiscard]]
    virtual glm::mat4 GetProjectionMatrix() const = 0;
    [[nodiscard]]
    virtual glm::mat4 GetModelMatrix() const = 0;
    [[nodiscard]]
    virtual glm::mat4 GetPVMMatrix() const = 0;
};

struct CameraInputReceiver : Input::InputObserver {
    void OnKeyEvent(const Input::KeyEvent& event) override = 0;
    void OnMouseButtonEvent(const Input::MouseButtonEvent& event) override = 0;
    void OnMouseMoveEvent(const Input::MouseMoveEvent& event) override = 0;
    void OnMouseScrollEvent(const Input::MouseScrollEvent& event) override = 0;
};

class Camera :
    public AbstractCamera,
    public CameraInputReceiver,
    public IUpdatable {
public:
    Camera();
    ~Camera() override;
    void Update() override;
    void SetPVMMatrix(const PVMMatrix& mvp) override;
    void SetRotateXYZ(const Eye& eye) override;
    void SetEyePosition(const Eye& eye) override;
    void SetFar(double far) override;
    void SetNear(double near) override;
    void SetAspectRatio(float aspectRatio) override;
    void SetFov(float fov) override;
    void SetZoom(float zoom) override;
    [[nodiscard]]
    float GetAspectRatio() const override;
    [[nodiscard]]
    glm::mat4 GetViewMatrix() const override;
    [[nodiscard]]
    glm::mat4 GetProjectionMatrix() const override;
    [[nodiscard]]
    glm::mat4 GetModelMatrix() const override;
    [[nodiscard]]
    glm::mat4 GetPVMMatrix() const override;
    void OnKeyEvent(const Input::KeyEvent& event) override;
    void OnMouseButtonEvent(const Input::MouseButtonEvent& event) override;
    void OnMouseMoveEvent(const Input::MouseMoveEvent& event) override;
    void OnMouseScrollEvent(const Input::MouseScrollEvent& event) override;
private:
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 pvmMatrix;
    void UpdateMatrices();
};

// Just for data interchange between classes
struct CameraStateResource : StateResource {
    Camera* cam;
    explicit CameraStateResource(Camera& camera) :
        cam(&camera)
    {
    }
};

struct CameraStateDrawableVulkan : StateDrawableVulkan {
    VkBuffer uniformBuffer = VK_NULL_HANDLE;
    VkDeviceMemory uniformBufferMemory = VK_NULL_HANDLE;
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    CameraStateDrawableVulkan() = default;
};

class CameraStateDrawable :
    StateDrawable<CameraStateResource, CameraStateDrawableVulkan>
{
public:
    /* Stores the base class type */
    using Base = StateDrawable;

    void OnDraw(const CameraStateResource& resource, CameraStateDrawableVulkan& vk,
                Game::VulkanContext& context) override;
    void OnUpdate(CameraStateResource& resource,
                  CameraStateDrawableVulkan& vk,
                  Game::VulkanContext& context) override;
    void OnCreate(CameraStateResource& resource,
                  CameraStateDrawableVulkan& vk,
                  Game::VulkanContext& context) override;
    void OnDestroy(CameraStateResource& resource,
                   CameraStateDrawableVulkan& vk,
                   Game::VulkanContext& context) override;
    void OnPause() override;
    void OnResume() override;
};

class CameraModel :
    public StateModel<Camera, CameraStateDrawable>
{
    std::shared_ptr<CameraStateDrawable> cameraDrawable;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<CameraStateResource> cameraResource;
    std::unique_ptr<CameraStateDrawableVulkan> cameraVk;
public:
    /* Constructs a model of the Camera class */
    explicit CameraModel(Game::VulkanContext& context);

    /* Gets the stored camera */
    Camera& GetCamera() const;

    /* Gets the stored camera */
    CameraStateResource& GetResource() override;

    /* Gets the stored camera */
    CameraStateDrawable& GetDrawable() override;

    /* Gets the stored camera */
    CameraStateDrawableVulkan& GetVulkanState() override;
};

} // namespace Rl::Providers
