cmake_minimum_required(VERSION 3.20)

# Disable vcpkg's POST_BUILD applocal copy step. We manage runtime assets
# explicitly below, and keeping this off avoids an extra PowerShell hook on
# every executable build.
set(VCPKG_APPLOCAL_DEPS OFF CACHE BOOL "Disable vcpkg applocal post-build dependency copying" FORCE)

project(Vulkan2DEngine LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(SCCACHE_PROGRAM sccache)
if(SCCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER "${SCCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${SCCACHE_PROGRAM}")
endif()

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
set(SANDBOX_SRC
    ${CMAKE_SOURCE_DIR}/Sandbox/Main.cpp
    ${CMAKE_SOURCE_DIR}/Sandbox/SandboxApp.cpp
    ${CMAKE_SOURCE_DIR}/Sandbox/SandboxApp.h
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
set(SANDBOX_ASSET_DIR ${CMAKE_BINARY_DIR}/Assets)
set(SHADERS
    ${SHADER_SRC_DIR}/Vertex.vert
    ${SHADER_SRC_DIR}/Fragment.frag
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
set(TEXTURE_ASSETS
    ${CMAKE_SOURCE_DIR}/Assets/Textures/character-spritesheet.png
    ${CMAKE_SOURCE_DIR}/Assets/Textures/gen-ec38b683-9381-40bb-bd92-a077a7cd4dbb.png
    ${CMAKE_SOURCE_DIR}/Assets/Textures/testspritesheet.png
    ${CMAKE_SOURCE_DIR}/Assets/Textures/texture.jpg
    ${CMAKE_SOURCE_DIR}/Assets/Textures/texture2.jpg
)

set(ANIMATION_ASSETS
    ${CMAKE_SOURCE_DIR}/Assets/Animations/CharacterSpriteSheet.csv
)

set(COPIED_TEXTURE_ASSETS "")
foreach(TEXTURE ${TEXTURE_ASSETS})
    get_filename_component(TEXTURE_NAME ${TEXTURE} NAME)
    set(TEXTURE_OUT ${SANDBOX_ASSET_DIR}/Textures/${TEXTURE_NAME})

    add_custom_command(
        OUTPUT ${TEXTURE_OUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SANDBOX_ASSET_DIR}/Textures
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEXTURE} ${TEXTURE_OUT}
        DEPENDS ${TEXTURE}
        COMMENT "Copying texture ${TEXTURE_NAME}"
        VERBATIM
    )

    list(APPEND COPIED_TEXTURE_ASSETS ${TEXTURE_OUT})
endforeach()

set(COPIED_ANIMATION_ASSETS "")
foreach(ANIMATION ${ANIMATION_ASSETS})
    get_filename_component(ANIMATION_NAME ${ANIMATION} NAME)
    set(ANIMATION_OUT ${SANDBOX_ASSET_DIR}/Animations/${ANIMATION_NAME})

    add_custom_command(
        OUTPUT ${ANIMATION_OUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SANDBOX_ASSET_DIR}/Animations
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ANIMATION} ${ANIMATION_OUT}
        DEPENDS ${ANIMATION}
        COMMENT "Copying animation ${ANIMATION_NAME}"
        VERBATIM
    )

    list(APPEND COPIED_ANIMATION_ASSETS ${ANIMATION_OUT})
endforeach()

add_custom_target(CopySandboxAssets
    DEPENDS
        ${COPIED_TEXTURE_ASSETS}
        ${COPIED_ANIMATION_ASSETS}
)

add_dependencies(CopySandboxAssets Shaders)
add_dependencies(Sandbox CopySandboxAssets)

# --- Organize shaders in IDE ---
source_group("Shaders" FILES ${SHADERS})
