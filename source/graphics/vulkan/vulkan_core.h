//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H
#include <vector>

#include "../graphics_core.h"
#include "vulkan/vulkan_core.h"

#endif //VULKAN_CORE_H

namespace ProtoCADGraphics {
    class VulkanAPI : public API {
    private:

        //validation layers
        VkInstance m_instance;
        VkInstanceCreateInfo m_createInfo;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkDebugUtilsMessengerCreateInfoEXT m_messageCreateInfo;

        const std::vector<const char*> m_validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        const bool m_enableValidationLayers = true;
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void SetupDebugMessenger();
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageType,const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,void* pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        bool CheckValidationLayerSupport();
        void CreateValidationLayers();
        std::vector<const char*> GetRequiredExtensions();

        //devices
        VkPhysicalDevice m_physicalDevice;

        int RateDeviceSuitability(VkPhysicalDevice device);

        bool IsDeviceSuitable(VkPhysicalDevice device);
        void PickPhysicalDevice();

    public:
        void Initialize(ProtoCADCore::Window* window) override;
        void CleanUp() override;
    };
}