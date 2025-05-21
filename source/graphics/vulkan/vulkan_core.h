//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H
#include "vulkan_pipeline.h"
#include "../graphics_objects.h"

#include <optional>
#include <vector>

#include "../graphics_core.h"
#include "vulkan/vulkan_core.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#endif //VULKAN_CORE_H


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

        uint32_t t_imageIndex;
        VkResult t_swapChainOutOfDateResult;
        bool m_recreateSwapChainHalt;

    public:
        uint32_t currentFrame = 0;

    private:
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
    public:
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);

    private:
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
        VkImageView CreateSingleImageView(VkImage image, VkFormat imageFormat, VkImageAspectFlags aspectMask);

        //framebuffers
        void CreateFrameBuffers();

        //command pools
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        void CreateCommandPool();
        void BeginRecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, Mesh mesh);
        void EndRecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void CreateCommandBuffers();

        VkCommandBuffer BeginSingleTimeCommands(VkCommandPool commandPool);
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool);

        //syncronization
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

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
        void UpdateUniformBuffer(uint32_t currentImage, glm::mat4 model, glm::mat4 view, float fov);

        //viewport
        std::vector<VkImage> m_ViewportImages;
        std::vector<VkDeviceMemory> m_DstImageMemory;
        std::vector<VkImageView> m_ViewportImageViews;
        std::vector<VkSampler> m_ViewportSamplers;

        void CreateViewportImage();
        void CreateViewportImageViews();
        void CreateViewportSamplers();

    public:
        void UpdateVertexBuffer(std::vector<Vertex> vertices);
        void UpdateIndexBuffer(std::vector<uint32_t> indices);

        void Initialize(std::shared_ptr<ProtoCADCore::Window> window, std::shared_ptr<ProtoCADScene::Scene> scene) override;
        void BeginDrawFrame(std::shared_ptr<ProtoCADScene::Scene> scene, glm::vec2 viewport) override;
        void EndDrawFrame() override;
        void CleanUp() override;

        int GetViewportRenderTexture() override;

        VkInstance GetInstance() { return m_instance; }
        std::vector<VkCommandBuffer> GetCommandBuffers() { return m_commandBuffers; }
        VkDevice GetDevice() { return m_device; }
        VkPhysicalDevice GetPhysicalDevice() { return m_physicalDevice; }
        VkSurfaceKHR GetSurface() { return m_surface; }
        VkSurfaceFormatKHR GetSurfaceFormat() { return ChooseSwapSurfaceFormat(QuerySwapChainSupport(m_physicalDevice).formats); }
        VkPresentModeKHR GetSurfacePresentMode() { return ChooseSwapPresentMode(QuerySwapChainSupport(m_physicalDevice).presentModes); }
        VkQueue GetGraphicsQueue() { return m_graphicsQueue; }
        VkQueue GetPresentQueue() { return m_presentQueue; }
        VkExtent2D GetExtent() { return m_swapChainExtent; }
        std::vector<VkFramebuffer> GetSwapChainFrameBuffers() { return m_swapChainFramebuffers; }
        uint32_t GetCurrentFrame() { return currentFrame; }
        std::shared_ptr<VulkanPipeline> GetCurrentPipeline() { return m_currentPipeline; }
        VkDescriptorPool GetDescriptorPool() { return m_descriptorPool; }
        uint32_t GetImageCount() { return m_swapChainImages.size(); }
        VkCommandPool GetCommandPool() { return m_commandPool; }
        VkSwapchainKHR GetSwapChainKHR() { return m_swapChain; }
        std::vector<VkImageView> GetImageViews() { return m_swapChainImageViews; }

        //vertex_handling
        static VkVertexInputBindingDescription GetBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
    };
}