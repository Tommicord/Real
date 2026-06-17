#include "rl/Base/CameraProvider.h"
#include "rl/Base/StateDrawable.h"
#include "rl/Base/ShaderFactory.h"
#include "rl/Base/Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vulkan/vulkan.h>

namespace Rl::Ui {

class VulkanCameraResources : public Providers::CameraStateDrawableResource {
public:
    VkBuffer uniformBuffer = VK_NULL_HANDLE;
    VkDeviceMemory uniformBufferMemory = VK_NULL_HANDLE;
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    Game::VulkanContext* vkContext = nullptr;
};

} // namespace Rl::Ui

namespace Rl::Providers {

void CameraStateDrawable::OnCreate(CameraStateDrawableResource resource)
{
    auto *vulkanResources = reinterpret_cast<Ui::VulkanCameraResources*>(&resource);
    vulkanResources->vkContext =
        &Game::Game::GetInstance().GetVulkanContext();
    // Create uniform buffer for PVM matrices
    constexpr VkDeviceSize bufferSize = sizeof(glm::mat4) * 3; // model view projection
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(vulkanResources->vkContext->device, &bufferInfo, nullptr, &vulkanResources->uniformBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create uniform buffer");
    }
    VkMemoryRequirements memRequirements{};
    vkGetBufferMemoryRequirements(vulkanResources->vkContext->device, vulkanResources->uniformBuffer, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = 0; // TODO: Find proper memory type
    
    if (vkAllocateMemory(vulkanResources->vkContext->device, &allocInfo, nullptr, &vulkanResources->uniformBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate uniform buffer memory");
    }
    vkBindBufferMemory(vulkanResources->vkContext->device, vulkanResources->uniformBuffer, vulkanResources->uniformBufferMemory, 0);
    // Create descriptor set layout
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;
    
    if (vkCreateDescriptorSetLayout(vulkanResources->vkContext->device, &layoutInfo, nullptr, &vulkanResources->descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor set layout");
    }
    
    // Store descriptor set layout in VulkanContext for pipeline creation
    vulkanResources->vkContext->descriptorSetLayout = vulkanResources->descriptorSetLayout;
    
    // Create descriptor pool
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 1;
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 1;
    
    if (vkCreateDescriptorPool(vulkanResources->vkContext->device, &poolInfo, nullptr, &vulkanResources->descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool");
    }
    // Create descriptor set
    VkDescriptorSetAllocateInfo allocInfo2{};
    allocInfo2.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo2.descriptorPool = vulkanResources->descriptorPool;
    allocInfo2.descriptorSetCount = 1;
    allocInfo2.pSetLayouts = &vulkanResources->descriptorSetLayout;
    
    if (vkAllocateDescriptorSets(vulkanResources->vkContext->device, &allocInfo2, &vulkanResources->descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets");
    }
    
    // Store descriptor set in VulkanContext for drawing
    vulkanResources->vkContext->descriptorSet = vulkanResources->descriptorSet;
    // Update descriptor set
    VkDescriptorBufferInfo bufferInfo2{};
    bufferInfo2.buffer = vulkanResources->uniformBuffer;
    bufferInfo2.offset = 0;
    bufferInfo2.range = bufferSize;
    
    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = vulkanResources->descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo2;
    vkUpdateDescriptorSets(vulkanResources->vkContext->device, 1, &descriptorWrite, 0, nullptr);
}

void CameraStateDrawable::OnDestroy(CameraStateDrawableResource resource)
{
    auto *vulkanResources = reinterpret_cast<Ui::VulkanCameraResources*>(&resource);
    Game::VulkanContext *context = vulkanResources->vkContext;
    vkDestroyDescriptorPool(context->device, vulkanResources->descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(context->device, vulkanResources->descriptorSetLayout, nullptr);
    vkDestroyBuffer(context->device, vulkanResources->uniformBuffer, nullptr);
    vkFreeMemory(context->device, vulkanResources->uniformBufferMemory, nullptr);
}

void CameraStateDrawable::OnPause()
{
    // Pause camera updates
}

void CameraStateDrawable::OnResume()
{
    // Resume camera updates
}

void CameraStateDrawable::OnUpdate(CameraStateDrawableResource resource)
{
    // Get camera matrices and upload to GPU
    auto *vulkanResources = reinterpret_cast<Ui::VulkanCameraResources*>(&resource);
    if (!vulkanResources || !vulkanResources->vkContext) {
        return;
    }
    // Get camera matrices from the camera instance
    glm::mat4 model = resource.cam_.GetModelMatrix();
    glm::mat4 view = resource.cam_.GetViewMatrix();
    glm::mat4 projection = resource.cam_.GetProjectionMatrix();
    // Upload matrices to uniform buffer
    const glm::mat4 matrices[3] = {model, view, projection};
    void* data;
    vkMapMemory(vulkanResources->vkContext->device, vulkanResources->uniformBufferMemory, 0, sizeof(matrices), 0, &data);
    memcpy(data, matrices, sizeof(matrices));
    vkUnmapMemory(vulkanResources->vkContext->device, vulkanResources->uniformBufferMemory);
}

void CameraStateDrawable::OnDraw()
{
    // Draw camera visualization
    // This would need access to the Vulkan context and command buffers
}

} // namespace Rl::Providers
