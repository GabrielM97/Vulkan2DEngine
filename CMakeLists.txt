cmake_minimum_required(VERSION 3.20)
project(Vulkan2DEngine)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vulkan SDK check
if (WIN32 AND NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable not set!")
endif ()

# Packages
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

# Engine dependencies
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

# Add engine modules
add_subdirectory(Engine)

# Sandbox executable
add_executable(Sandbox Sandbox/main.cpp)
target_link_libraries(Sandbox
        Core
        Platform
        Renderer
        EngineDeps
)
target_include_directories(Sandbox PUBLIC Engine)

# --- Shader Compilation ---
find_program(GLSLC glslc REQUIRED)

set(SHADER_SRC_DIR ${CMAKE_SOURCE_DIR}/Assets/Shaders)
set(SHADER_OUT_DIR ${CMAKE_BINARY_DIR}/Debug/Assets/Shaders)
file(GLOB SHADERS ${SHADER_SRC_DIR}/*.vert ${SHADER_SRC_DIR}/*.frag)

file(GLOB SHADERS CONFIGURE_DEPENDS
        ${SHADER_SRC_DIR}/*.vert
        ${SHADER_SRC_DIR}/*.frag
)

set(SPIRV_BINARIES "")
foreach (SHADER ${SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(OUT_FILE ${SHADER_OUT_DIR}/${FILE_NAME}.spv)

    add_custom_command(
            OUTPUT ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUT_DIR}
            COMMAND ${GLSLC} ${SHADER} -o ${OUT_FILE}
            DEPENDS ${SHADER}
            COMMENT "Compiling shader ${FILE_NAME}"
            VERBATIM
    )

    list(APPEND SPIRV_BINARIES ${OUT_FILE})
endforeach ()

# Custom target to build all shaders
add_custom_target(Shaders ALL DEPENDS ${SPIRV_BINARIES})
add_dependencies(Sandbox Shaders)

# Copy shaders to Sandbox output folder after build
add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Sandbox>/Assets/Shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_OUT_DIR} $<TARGET_FILE_DIR:Sandbox>/Assets/Shaders
        COMMENT "Copying compiled shaders to executable folder"
)

# --- Make shader folder visible in IDE ---
file(GLOB_RECURSE SHADERS
        ${CMAKE_SOURCE_DIR}/Assets/Shaders/*.vert
        ${CMAKE_SOURCE_DIR}/Assets/Shaders/*.frag
)

# Create a dummy target so IDE shows them
add_custom_target(ShadersFiles ALL
        SOURCES ${SHADERS}
)

# --- Make shaders visible in IDE ---
source_group("Shaders" FILES ${SHADERS})