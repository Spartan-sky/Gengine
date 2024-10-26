#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <optional>
#include <string.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
                return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
                func(instance, debugMessenger, pAllocator);
        }
}

class HelloTriangleApplication
{
public:
        void run()
        {
                initWindow();
                initVulkan();
                mainLoop();
                cleanup();
        }

private:
        GLFWwindow *window;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        struct QueueFamilyIndices
        {
                std::optional<uint32_t> graphicsFamily;

                bool isComplete()
                {
                        return graphicsFamily.has_value();
                }
        };

        void
        initWindow()
        {
                glfwInit();

                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

                window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        }

        void initVulkan()
        {
                createInstance();
                setupDebugMessenger();
                pickPhysicalDevice();
                // createLogicalDevice();
        }

        void pickPhysicalDevice()
        {
                uint32_t deviceCount = 0;
                vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

                if (deviceCount == 0)
                        throw std::runtime_error("failed to find GPUs with Vulkan support!");

                std::vector<VkPhysicalDevice> devices(deviceCount);
                vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

                for (const auto &device : devices)
                {
                        if (isDeviceSuitable(device))
                        {
                                physicalDevice = device;
                                break;
                        }
                }

                if (physicalDevice == VK_NULL_HANDLE)
                        throw std::runtime_error("failed to find a suitable GPU!");
        }

        bool isDeviceSuitable(VkPhysicalDevice device)
        {
                QueueFamilyIndices indices = findQueueFamilies(device);

                return indices.isComplete();
        }

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
        {
                QueueFamilyIndices indices;

                uint32_t queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

                std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

                int i = 0;
                for (const auto &queueFamily : queueFamilies)
                {
                        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                        {
                                indices.graphicsFamily = i;
                        }

                        if (indices.isComplete())
                                break;

                        i++;
                }

                return indices;
        }

        void mainLoop()
        {
                while (!glfwWindowShouldClose(window))
                {
                        glfwPollEvents();
                }
        }

        void cleanup()
        {
                if (enableValidationLayers)
                {
                        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
                }

                vkDestroyInstance(instance, nullptr);

                glfwDestroyWindow(window);

                glfwTerminate();
        }

        void createInstance()
        {
                if (enableValidationLayers && !checkValidationLayerSupport())
                {
                        throw std::runtime_error("validation layers requested, but not available!");
                }

                VkApplicationInfo appInfo{};
                appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                appInfo.pApplicationName = "Hello Triangle";
                appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo.pEngineName = "No Engine";
                appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo.apiVersion = VK_API_VERSION_1_0;

                VkInstanceCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                createInfo.pApplicationInfo = &appInfo;

                auto extensions = getRequiredExtensions();
                createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
                createInfo.ppEnabledExtensionNames = extensions.data();

                VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
                if (enableValidationLayers)
                {
                        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                        createInfo.ppEnabledLayerNames = validationLayers.data();

                        populateDebugMessengerCreateInfo(debugCreateInfo);
                        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
                }
                else
                {
                        createInfo.enabledLayerCount = 0;

                        createInfo.pNext = nullptr;
                }

                if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
                {
                        throw std::runtime_error("failed to create instance!");
                }
        }

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
        {
                createInfo = {};
                createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                createInfo.pfnUserCallback = debugCallback;
        }

        void setupDebugMessenger()
        {
                if (!enableValidationLayers)
                        return;

                VkDebugUtilsMessengerCreateInfoEXT createInfo;
                populateDebugMessengerCreateInfo(createInfo);

                if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
                {
                        throw std::runtime_error("failed to set up debug messenger!");
                }
        }

        std::vector<const char *> getRequiredExtensions()
        {
                uint32_t glfwExtensionCount = 0;
                const char **glfwExtensions;
                glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

                std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

                if (enableValidationLayers)
                {
                        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                }

                return extensions;
        }

        bool checkValidationLayerSupport()
        {
                uint32_t layerCount;
                vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

                std::vector<VkLayerProperties> availableLayers(layerCount);
                vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

                for (const char *layerName : validationLayers)
                {
                        bool layerFound = false;

                        for (const auto &layerProperties : availableLayers)
                        {
                                if (strcmp(layerName, layerProperties.layerName) == 0)
                                {
                                        layerFound = true;
                                        break;
                                }
                        }

                        if (!layerFound)
                        {
                                return false;
                        }
                }

                return true;
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
        {
                std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

                return VK_FALSE;
        }
};

int main()
{
        HelloTriangleApplication app;

        try
        {
                app.run();
        }
        catch (const std::exception &e)
        {
                std::cerr << e.what() << std::endl;
                return EXIT_FAILURE;
        }

        //         {
        // #if defined(__clang__)
        //                 std::cout << "Compiler: Clang\n";
        //                 std::cout << "Version: " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << "\n";
        // #elif defined(__GNUC__) || defined(__GNUG__)
        //                 std::cout << "Compiler: GCC\n";
        //                 std::cout << "Version: " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << "\n";
        // #elif defined(_MSC_VER)
        //                 std::cout << "Compiler: MSVC\n";
        //                 std::cout << "Version: " << _MSC_VER << "\n";
        // #else
        //                 std::cout << "Unknown compiler\n";
        // #endif

        // #if __cplusplus == 199711L
        //                 std::cout << "C++ version: C++98 or C++03\n";
        // #elif __cplusplus == 201103L
        //                 std::cout << "C++ version: C++11\n";
        // #elif __cplusplus == 201402L
        //                 std::cout << "C++ version: C++14\n";
        // #elif __cplusplus == 201703L
        //                 std::cout << "C++ version: C++17\n";
        // #elif __cplusplus == 202002L
        //                 std::cout << "C++ version: C++20\n";
        // #elif __cplusplus > 202002L
        //                 std::cout << "C++ version: C++23 or newer\n";
        // #else
        //                 std::cout << "C++ version: pre-standard C++\n";
        // #endif
        //         }

        return EXIT_SUCCESS;
}