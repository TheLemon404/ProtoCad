//
// Created by Osprey on 5/16/2025.
//

#pragma once

#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#endif //VULKAN_PIPELINE_H

#include <vector>

#include "vulkan/vulkan_core.h"
#include "../graphics_objects.h"

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
        VkClearValue m_clearColor;

        VkRenderPass GetRenderPass() { return m_renderPass; }

        VulkanPipeline(VkDevice device, VkExtent2D swapChainExtent, VkFormat renderPassFormat, const char* vertexShaderPath, const char* fragmentShaderPath);

        VkPipeline GetPipeline() { return m_graphicsPipeline; }

        void BeingRenderPass(VkFramebuffer* swapChainFramebuffer, VkExtent2D swapChainExtent, VkCommandBuffer commandBuffer, VkBuffer vertexBuffer, std::vector<Vertex> vertices);
        void CleanUp();
    };

    class UnlitVulkanPipeline : public VulkanPipeline {
    public:
        UnlitVulkanPipeline(VkDevice device, VkExtent2D swapChainExtent, VkFormat renderPassFormat);
    };

    class LitVulkanPipeline : public VulkanPipeline {
    public:
        LitVulkanPipeline(VkDevice device, VkExtent2D swapChainExtent, VkFormat renderPassFormat);
    };
}