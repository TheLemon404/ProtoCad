//
// Created by Osprey on 5/16/2025.
//

#pragma once

#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include <memory>
#include <vector>

#include "vulkan/vulkan_core.h"
#include "../graphics_objects.h"

#endif //VULKAN_PIPELINE_H

namespace ProtoCADGraphics {
    class VulkanPipeline {
    protected:
        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        VkDevice p_device;
        VkFormat p_swapChainImageFormat;

        VkRenderPass m_renderPass;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_graphicsPipeline;

        VkShaderModule m_vertShaderModule;
        VkShaderModule m_fragShaderModule;

    public:
        VkClearValue clearColor;

        VkRenderPass GetRenderPass() { return m_renderPass; }

        VulkanPipeline(VkDevice device, VkExtent2D swapChainExtent, VkFormat renderPassFormat, const char* vertexShaderPath, const char* fragmentShaderPath, VkDescriptorSetLayout descriptorSetLayout);

        VkPipeline GetPipeline() { return m_graphicsPipeline; }

        void BeingRenderPass(VkFramebuffer* swapChainFramebuffer, VkExtent2D swapChainExtent, VkCommandBuffer commandBuffer, VkBuffer vertexBuffer, VkBuffer indexBuffer, Mesh mesh, std::vector<VkDescriptorSet> descriptorSets, uint32_t currentFrame);
        void EndRenderPass(VkCommandBuffer commandBuffer);
        void CleanUp();
    };

    class UnlitVulkanPipeline : public VulkanPipeline {
    public:
        UnlitVulkanPipeline(VkDevice device, VkExtent2D swapChainExtent, VkFormat renderPassFormat, VkDescriptorSetLayout descriptorSetLayout);
    };

    class LitVulkanPipeline : public VulkanPipeline {
    public:
        LitVulkanPipeline(VkDevice device, VkExtent2D swapChainExtent, VkFormat renderPassFormat, VkDescriptorSetLayout descriptorSetLayout);
    };
}