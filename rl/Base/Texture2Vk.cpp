#include "rl/Base/Texture2.h"

#include "rl/Base/Game.h"

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstring>

namespace Rl::Providers {

VkFormat Texture2::GetVkFormat() const
{
    switch (properties.format) {
        case TextureFormat::RGB8:
            return VK_FORMAT_R8G8B8_UNORM;
        case TextureFormat::RGBA8:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::RGB16F:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case TextureFormat::RGBA16F:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case TextureFormat::RGB32F:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case TextureFormat::RGBA32F:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case TextureFormat::R8:
            return VK_FORMAT_R8_UNORM;
        case TextureFormat::RG8:
            return VK_FORMAT_R8G8_UNORM;
        case TextureFormat::R16F:
            return VK_FORMAT_R16_SFLOAT;
        case TextureFormat::RG16F:
            return VK_FORMAT_R16G16_SFLOAT;
        case TextureFormat::DEPTH16:
            return VK_FORMAT_D16_UNORM;
        case TextureFormat::DEPTH24:
            return VK_FORMAT_X8_D24_UNORM_PACK32;
        case TextureFormat::DEPTH32F:
            return VK_FORMAT_D32_SFLOAT;
        default:
            return VK_FORMAT_R8G8B8A8_UNORM;
    }
}

VkFilter Texture2::GetVkFilter(TextureFilter filter) const
{
    switch (filter) {
        case TextureFilter::NEAREST:
            return VK_FILTER_NEAREST;
        case TextureFilter::LINEAR:
            return VK_FILTER_LINEAR;
        case TextureFilter::NEAREST_MIPMAP_NEAREST:
            return VK_FILTER_NEAREST;
        case TextureFilter::LINEAR_MIPMAP_NEAREST:
            return VK_FILTER_LINEAR;
        case TextureFilter::NEAREST_MIPMAP_LINEAR:
            return VK_FILTER_NEAREST;
        case TextureFilter::LINEAR_MIPMAP_LINEAR:
            return VK_FILTER_LINEAR;
        default:
            return VK_FILTER_LINEAR;
    }
}

VkSamplerMipmapMode Texture2::GetVkMipmapMode(TextureFilter filter) const
{
    switch (filter) {
        case TextureFilter::NEAREST:
        case TextureFilter::LINEAR:
            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case TextureFilter::NEAREST_MIPMAP_NEAREST:
        case TextureFilter::LINEAR_MIPMAP_NEAREST:
            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case TextureFilter::NEAREST_MIPMAP_LINEAR:
        case TextureFilter::LINEAR_MIPMAP_LINEAR:
            return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        default:
            return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    }
}

VkSamplerAddressMode Texture2::GetVkWrapMode(TextureWrap wrap) const
{
    switch (wrap) {
        case TextureWrap::REPEAT:
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case TextureWrap::MIRRORED_REPEAT:
            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case TextureWrap::CLAMP_TO_EDGE:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case TextureWrap::CLAMP_TO_BORDER:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        default:
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    }
}

void Texture2::CreateVulkanImage(Game::VulkanContext& context)
{
    if (!loaded || binding.vkImage != VK_NULL_HANDLE) {
        return;
    }

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = static_cast<uint32_t>(mipmapLevels);
    imageInfo.arrayLayers = 1;
    imageInfo.format = GetVkFormat();
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(context.device, &imageInfo, nullptr, &binding.vkImage) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan image");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(context.device, binding.vkImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = 0; // TODO: Find appropriate memory type

    if (vkAllocateMemory(context.device, &allocInfo, nullptr, &binding.mvkImageMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate Vulkan image memory");
    }

    vkBindImageMemory(context.device, binding.vkImage, binding.mvkImageMemory, 0);
}

void Texture2::UploadTextureData(Game::VulkanContext& context)
{
    if (!loaded || data == nullptr || binding.vkImage == VK_NULL_HANDLE) {
        return;
    }

    // Create staging buffer
    VkBufferCreateInfo stagingBufferInfo{};
    stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingBufferInfo.size = dataSize;
    stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(context.device, &stagingBufferInfo, nullptr, &binding.vkStagingBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create staging buffer");
    }

    VkMemoryRequirements stagingMemRequirements;
    vkGetBufferMemoryRequirements(context.device, binding.vkStagingBuffer, &stagingMemRequirements);

    VkMemoryAllocateInfo stagingAllocInfo{};
    stagingAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    stagingAllocInfo.allocationSize = stagingMemRequirements.size;
    stagingAllocInfo.memoryTypeIndex = 0; // TODO: Find appropriate memory type

    if (vkAllocateMemory(context.device, &stagingAllocInfo, nullptr, &binding.vkStagingBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate staging buffer memory");
    }

    vkBindBufferMemory(context.device, binding.vkStagingBuffer, binding.vkStagingBufferMemory, 0);

    // Copy texture data to staging buffer
    void* data;
    vkMapMemory(context.device, binding.vkStagingBufferMemory, 0, dataSize, 0, &data);
    memcpy(data, data, dataSize);
    vkUnmapMemory(context.device, binding.vkStagingBufferMemory);

    // TODO: Transition image layout and copy buffer to image
    // This requires command buffer recording which needs a command pool
}

void Texture2::CreateVulkanSampler(Game::VulkanContext& context)
{
    if (binding.vkSampler != VK_NULL_HANDLE) {
        return;
    }

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = GetVkFilter(properties.magFilter);
    samplerInfo.minFilter = GetVkFilter(properties.minFilter);
    samplerInfo.addressModeU = GetVkWrapMode(properties.wrapS);
    samplerInfo.addressModeV = GetVkWrapMode(properties.wrapT);
    samplerInfo.addressModeW = GetVkWrapMode(properties.wrapT);
    samplerInfo.anisotropyEnable = properties.anisotropicFiltering ? VK_TRUE : VK_FALSE;
    samplerInfo.maxAnisotropy = properties.maxAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = GetVkMipmapMode(properties.minFilter);
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = static_cast<float>(mipmapLevels);

    if (vkCreateSampler(context.device, &samplerInfo, nullptr, &binding.vkSampler) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan sampler");
    }
}

void Texture2::GetSampler(Game::VulkanContext& context)
{
    if (binding.vkSampler == VK_NULL_HANDLE) {
        CreateVulkanSampler(context);
    }
}

void Texture2::GetImageView(Game::VulkanContext& context)
{
    if (binding.vkImageView != VK_NULL_HANDLE) {
        return;
    }

    if (binding.vkImage == VK_NULL_HANDLE) {
        CreateVulkanImage(context);
        UploadTextureData(context);
    }

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = binding.vkImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = GetVkFormat();
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = static_cast<uint32_t>(mipmapLevels);
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(context.device, &viewInfo, nullptr, &binding.vkImageView) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan image view");
    }
}

}