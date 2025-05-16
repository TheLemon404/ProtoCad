//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H
#include "vulkan_pipeline.h"

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

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanAPI : public API {
    private:
        GLFWwindow* p_window;

        //validation layers
        VkInstance m_instance;
        VkInstanceCreateInfo m_createInfo;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkDebugUtilsMessengerCreateInfoEXT m_messageCreateInfo;

        std::shared_ptr<VulkanPipeline> m_pipeline;

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
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        void PickPhysicalDevice();

        //logical devices
        VkDevice m_device;

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        void CreateLogicalDevice();

        //queue
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);

        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;


        //surface KHR
        VkSurfaceKHR m_surface;
        void CreateSurface(std::shared_ptr<ProtoCADCore::Window> window);

        //swap chain
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        void CreateSwapChain();

        //image views
        std::vector<VkImageView> m_swapChainImageViews;

        void CreateImageViews();

    public:
        void Initialize(std::shared_ptr<ProtoCADCore::Window> window) override;
        void CleanUp() override;
    };
}