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
