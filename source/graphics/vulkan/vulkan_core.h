//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H

#endif //VULKAN_CORE_H

#include <optional>
#include <vector>

#include "../graphics_core.h"
#include "vulkan/vulkan_core.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace ProtoCADGraphics {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool IsComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

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

        //physical devices
        VkPhysicalDevice m_physicalDevice;

        int RateDeviceSuitability(VkPhysicalDevice device);

        void PickPhysicalDevice();

        //logical devices
        VkDevice m_device;

        void CreateLogicalDevice();

        //queue
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);

        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;


        //surface KHR
        VkSurfaceKHR m_surface;
        void CreateSurface(ProtoCADCore::Window* window);

    public:
        void Initialize(ProtoCADCore::Window* window) override;
        void CleanUp() override;
    };
}