cmake_minimum_required(VERSION 3.10)

project(VulkanEngine)

add_executable(VulkanEngine main.cpp)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(include/glfw-3.4)

target_link_libraries(VulkanEngine glfw)

# GLM
add_subdirectory(include/glm-1.0.1)

target_link_libraries(VulkanEngine glm::glm)

# Vulkan
find_package(Vulkan REQUIRED)

target_link_libraries(VulkanEngine Vulkan::Vulkan)