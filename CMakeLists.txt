cmake_minimum_required(VERSION 3.20)
project(Vulkan2DEngine LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Vulkan SDK check ---
if (WIN32 AND NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable not set!")
endif()

# --- Find packages via vcpkg ---
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

# --- Engine dependencies ---
add_library(EngineDeps INTERFACE)
if(WIN32)
    target_include_directories(EngineDeps INTERFACE
        $ENV{VCPKG_ROOT}/installed/x64-windows/include
        $ENV{VULKAN_SDK}/Include
        ${CMAKE_SOURCE_DIR}/External/stb
    )
elseif(UNIX)
    target_include_directories(EngineDeps INTERFACE
        $ENV{VCPKG_ROOT}/installed/x64-linux/include
        /usr/include
         ${CMAKE_SOURCE_DIR}/External/stb
    )
endif()
target_link_libraries(EngineDeps INTERFACE
    Vulkan::Vulkan
    glfw
    glm::glm
    imgui::imgui
    EnTT::EnTT
)

# --- Add engine modules ---
add_subdirectory(Engine)

# --- Sandbox executable ---
file(GLOB_RECURSE SANDBOX_SRC
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/Sandbox/*.cpp
    ${CMAKE_SOURCE_DIR}/Sandbox/*.h
)

add_executable(Sandbox ${SANDBOX_SRC})
target_link_libraries(Sandbox
    Core
    Platform
    Renderer
    Scene
    EngineDeps
)
target_include_directories(Sandbox PUBLIC Engine)

target_sources(Renderer PUBLIC
    ${CMAKE_SOURCE_DIR}/External/stb/stb_image.cpp
    ${CMAKE_SOURCE_DIR}/External/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/External/imgui/backends/imgui_impl_vulkan.cpp
)

target_include_directories(EngineDeps INTERFACE
    ${CMAKE_SOURCE_DIR}/External/imgui
)

# --- Shader compilation ---
find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)

set(SHADER_SRC_DIR ${CMAKE_SOURCE_DIR}/Assets/Shaders)
set(SHADER_OUT_DIR ${CMAKE_BINARY_DIR}/Assets/Shaders)
file(GLOB_RECURSE SHADERS CONFIGURE_DEPENDS
    ${SHADER_SRC_DIR}/*.vert
    ${SHADER_SRC_DIR}/*.frag
)

set(SPIRV_BINARIES "")
foreach(SHADER ${SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(OUT_FILE ${SHADER_OUT_DIR}/${FILE_NAME}.spv)

    add_custom_command(
        OUTPUT ${OUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUT_DIR}
        COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER} -o ${OUT_FILE}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${FILE_NAME}"
        VERBATIM
    )

    list(APPEND SPIRV_BINARIES ${OUT_FILE})
endforeach()

add_custom_target(Shaders ALL DEPENDS ${SPIRV_BINARIES})
add_dependencies(Sandbox Shaders)

# --- Copy runtime assets to executable folder ---
add_custom_target(CopySandboxAssets ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Sandbox>/Assets/Shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SHADER_OUT_DIR}
            $<TARGET_FILE_DIR:Sandbox>/Assets/Shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Sandbox>/Assets/Textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Assets/Textures
            $<TARGET_FILE_DIR:Sandbox>/Assets/Textures
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Sandbox>/Assets/Animations
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Assets/Animations
            $<TARGET_FILE_DIR:Sandbox>/Assets/Animations
    COMMENT "Copying shaders, textures, and animations to executable folder"
)

add_dependencies(CopySandboxAssets Shaders)
add_dependencies(Sandbox CopySandboxAssets)

# --- Organize shaders in IDE ---
source_group("Shaders" FILES ${SHADERS})
