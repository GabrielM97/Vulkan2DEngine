cmake_minimum_required(VERSION 3.20)
project(Vulkan2DEngine)

# 2️⃣ C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3️⃣ Vulkan SDK check
if(WIN32 AND NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable not set. Please install Vulkan SDK and set VULKAN_SDK.")
endif()

# 4️⃣ Find packages (vcpkg)
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

# 5️⃣ Make a global interface target for vcpkg libraries
add_library(EngineDeps INTERFACE)
target_include_directories(EngineDeps INTERFACE
    $ENV{VCPKG_ROOT}/installed/x64-windows/include
    $ENV{VULKAN_SDK}/Include
)
target_link_libraries(EngineDeps INTERFACE
    Vulkan::Vulkan
    glfw
    glm::glm
    imgui::imgui
    EnTT::EnTT
)

# 6️⃣ Add engine modules
add_subdirectory(Engine)

# 7️⃣ Sandbox executable
add_executable(Sandbox Sandbox/main.cpp)

# 8️⃣ Link Sandbox against engine modules + engine-wide deps
target_link_libraries(Sandbox
    Core
    Platform
    Renderer
    EngineDeps
)

# 9️⃣ Include VMA (header-only)
target_include_directories(Sandbox PRIVATE
    $ENV{VCPKG_ROOT}/installed/x64-windows/include/vma
)

target_include_directories(Sandbox PUBLIC Engine)