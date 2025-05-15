//
// Created by Osprey on 5/15/2025.
//

#include "vulkan_core.h"

#include <map>
#include <string.h>
#include <vector>

#include "vulkan/vulkan.h"

#include "../../core/logging.h"

namespace ProtoCADGraphics {
    bool VulkanAPI::CheckValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    void VulkanAPI::CreateValidationLayers() {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (m_enableValidationLayers) {
            m_createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
            m_createInfo.ppEnabledLayerNames = m_validationLayers.data();

            PopulateDebugMessengerCreateInfo(debugCreateInfo);
            m_createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } else {
            m_createInfo.enabledLayerCount = 0;

            m_createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&m_createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            ProtoCADCore::Logging::Error("failed to create instance!");
        }
    }

    std::vector<const char*> VulkanAPI::GetRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (m_enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void VulkanAPI::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
    }

    void VulkanAPI::SetupDebugMessenger() {
        if (!m_enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        PopulateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
            ProtoCADCore::Logging::Error("failed to set up debug messenger!");
        }
    }

    VkResult VulkanAPI::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanAPI::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanAPI::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageType,const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        ProtoCADCore::Logging::Warn( "validation layer: " + std::string(pCallbackData->pMessage));

        return VK_FALSE;
    }

    int VulkanAPI::RateDeviceSuitability(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader) {
            return 0;
        }

        return score;
    }


    void VulkanAPI::PickPhysicalDevice() {
        m_physicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            ProtoCADCore::Logging::Error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            int score = RateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first > 0) {
            m_physicalDevice = candidates.rbegin()->second;
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);

            ProtoCADCore::Logging::Log("picked device: " + std::string(deviceProperties.deviceName));
            ProtoCADCore::Logging::Log("picked device api version: " + std::to_string(deviceProperties.apiVersion));
            ProtoCADCore::Logging::Log("picked device type: " + std::to_string(deviceProperties.deviceType));

        } else {
            ProtoCADCore::Logging::Error("failed to find a suitable GPU!");
        }
    }


    void VulkanAPI::Initialize(ProtoCADCore::Window* window) {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "ProtoCAP";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "ProtoCAD Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        m_createInfo = {};
        m_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_createInfo.pApplicationInfo = &appInfo;

        m_createInfo.enabledExtensionCount = window->glfwExtensionCount;
        m_createInfo.ppEnabledExtensionNames = window->glfwExtensions;
        m_createInfo.enabledLayerCount = 0;

        VkResult result = vkCreateInstance(&m_createInfo, nullptr, &m_instance);
        if (vkCreateInstance(&m_createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            ProtoCADCore::Logging::Error("failed to create instance!");
        }

        std::vector<const char*> requiredExtensions;

        for(uint32_t i = 0; i < window->glfwExtensionCount; i++) {
            requiredExtensions.emplace_back(window->glfwExtensions[i]);
        }

        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

        m_createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        m_createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        m_createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        if (vkCreateInstance(&m_createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            ProtoCADCore::Logging::Error("failed to create instance!");
        }

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        ProtoCADCore::Logging::Log("available extensions:\n");

        for (const auto& extension : extensions) {
            ProtoCADCore::Logging::Log(extension.extensionName + '\n');
        }

        if (m_enableValidationLayers && !CheckValidationLayerSupport()) {
            ProtoCADCore::Logging::Error("validation layers requested, but not available!");
        }

        if (m_enableValidationLayers) {
            m_createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
            m_createInfo.ppEnabledLayerNames = m_validationLayers.data();
        } else {
            m_createInfo.enabledLayerCount = 0;
        }

        CreateValidationLayers();
        PickPhysicalDevice();
    }

    void VulkanAPI::CleanUp() {
        if (m_enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
        }

        vkDestroyInstance(m_instance, nullptr);
    }
}
