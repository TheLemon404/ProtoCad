//
// Created by Osprey on 5/16/2025.
//

#include "vulkan_pipeline.h"

#include "../../core/io.h"

namespace ProtoCADGraphics {
    VkShaderModule VulkanPipeline::CreateShaderModule(const std::vector<char> &code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(p_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }

    VulkanPipeline::VulkanPipeline(VkDevice device) {
        p_device = device;

        auto vertShaderCode = ProtoCADCore::IO::ReadFile("/assets/shaders/vert.spv");
        auto fragShaderCode = ProtoCADCore::IO::ReadFile("/assets/shaders/frag.spv");

        vertShaderModule = CreateShaderModule(vertShaderCode);
        fragShaderModule = CreateShaderModule(fragShaderCode);
    }

    void VulkanPipeline::CleanUp() {
        vkDestroyShaderModule(p_device, fragShaderModule, nullptr);
        vkDestroyShaderModule(p_device, vertShaderModule, nullptr);
    }
}
