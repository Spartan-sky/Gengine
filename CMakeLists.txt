cmake_minimum_required(VERSION 3.10)

project(GEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

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