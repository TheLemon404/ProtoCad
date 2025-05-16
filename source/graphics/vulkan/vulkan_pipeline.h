//
// Created by Osprey on 5/16/2025.
//

#pragma once

#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H
#include <memory>
#include <vector>

#include "vulkan/vulkan_core.h"

#endif //VULKAN_PIPELINE_H

namespace ProtoCADGraphics {
    class VulkanPipeline {
    protected:
        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        VkDevice p_device;

        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;

    public:
        VulkanPipeline(VkDevice device);

        void CleanUp();
    };
}