//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H
#include "vulkan_pipeline.h"
#include "../graphics_objects.h"

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

    class VulkanAPI : public GraphicsAPI {
    private:
        GLFWwindow* p_window;

        const int MAX_FRAMES_IN_FLIGHT = 2;
        uint32_t currentFrame = 0;

        //validation layers
        VkInstance m_instance;
        VkInstanceCreateInfo m_createInfo;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkDebugUtilsMessengerCreateInfoEXT m_messageCreateInfo;

        std::shared_ptr<VulkanPipeline> m_currentPipeline;

        std::vector<VkFramebuffer> m_swapChainFramebuffers;

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
        void CleanUpSwapChain();
        void ReCreateSwapChain();

        //image views
        std::vector<VkImageView> m_swapChainImageViews;

        void CreateImageViews();

        //framebuffers
        void CreateFrameBuffers();

        //command pools
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        void CreateCommandPool();
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, Mesh mesh);
        void CreateCommandBuffers();

        //syncronization
        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        bool frameBufferResized = false;
        void CreateSyncObjects();

        //vertex buffers
        VkBuffer m_vertexBuffer;
        VkBuffer m_indexBuffer;
        VkDeviceMemory m_vertexBufferMemory;
        VkDeviceMemory m_indexBufferMemory;

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void CreateVertexBuffer(std::vector<Vertex> vertices);
        void CreateIndexBuffer(std::vector<uint32_t> indices);
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        //descriptor sets
        VkDescriptorSetLayout m_descriptorSetLayout;

        std::vector<VkBuffer> m_uniformBuffers;
        std::vector<VkDeviceMemory> m_uniformBuffersMemory;
        std::vector<void*> m_uniformBuffersMapped;
        VkDescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;

        void CreateDescriptorSetLayout();
        void CreateUniformBuffers();
        void CreateDescriptorPool();
        void CreateDescriptorSets();
        void UpdateUniformBuffer(uint32_t currentImage);

    public:
        void UpdateVertexBuffer(std::vector<Vertex> vertices);
        void UpdateIndexBuffer(std::vector<uint32_t> indices);

    public:
        void HandleWindowResize() override;
        void Initialize(std::shared_ptr<ProtoCADCore::Window> window, Mesh mesh) override;
        void DrawFrame(Mesh mesh) override;
        void CleanUp() override;

        //vertex_handling
        static VkVertexInputBindingDescription GetBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
    };
}