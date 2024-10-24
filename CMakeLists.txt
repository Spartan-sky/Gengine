cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(GEngine)


add_executable(GEngine main.cpp)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(include/glfw-3.4)

target_link_libraries(GEngine glfw)

# GLM
add_subdirectory(include/glm-1.0.1)

target_link_libraries(GEngine glm::glm)

# Vulkan
find_package(Vulkan REQUIRED)

target_link_libraries(GEngine Vulkan::Vulkan)