# --- Core ---
set(CORE_SRC
    Core/Application.cpp
    Core/Application.h
    Math/Transform2D.h
    Math/TransformMath2D.cpp
    Math/TransformMath2D.h
)

add_library(Core STATIC ${CORE_SRC})
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Core PUBLIC EngineDeps)

# --- Platform ---
set(PLATFORM_SRC
    Platform/Window.cpp
    Platform/Window.h
)

add_library(Platform STATIC ${PLATFORM_SRC})
target_include_directories(Platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Platform PUBLIC Core EngineDeps)

# --- Renderer ---
set(RENDERER_SRC
    Renderer/ImGuiLayer.cpp
    Renderer/ImGuiLayer.h
    Renderer/IRenderer2D.cpp
    Renderer/IRenderer2D.h
    Renderer/RenderTypes.h
    Renderer/VulkanCommandBuffer.cpp
    Renderer/VulkanCommandBuffer.h
    Renderer/VulkanContext.cpp
    Renderer/VulkanContext.h
    Renderer/VulkanDevice.cpp
    Renderer/VulkanDevice.h
    Renderer/VulkanFramebuffer.cpp
    Renderer/VulkanFramebuffer.h
    Renderer/VulkanIndexBuffer.cpp
    Renderer/VulkanIndexBuffer.h
    Renderer/VulkanPipeline.cpp
    Renderer/VulkanPipeline.h
    Renderer/VulkanQuadInstanceBuffer.cpp
    Renderer/VulkanQuadInstanceBuffer.h
    Renderer/VulkanRenderer.cpp
    Renderer/VulkanRenderer.h
    Renderer/VulkanRenderPass.cpp
    Renderer/VulkanRenderPass.h
    Renderer/VulkanSwapChain.cpp
    Renderer/VulkanSwapChain.h
    Renderer/VulkanSync.cpp
    Renderer/VulkanSync.h
    Renderer/VulkanTexture.cpp
    Renderer/VulkanTexture.h
    Renderer/VulkanUniformBuffer.cpp
    Renderer/VulkanUniformBuffer.h
    Renderer/VulkanVertexBuffer.cpp
    Renderer/VulkanVertexBuffer.h
)

add_library(Renderer STATIC ${RENDERER_SRC})
target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Renderer PUBLIC Core Platform EngineDeps)

# --- Scene ---
set(SCENE_SRC
    Scene/Camera2D.cpp
    Scene/Camera2D.h
    Scene/Entity.cpp
    Scene/Entity.h
    Scene/Scene.cpp
    Scene/Scene.h
    Scene/SceneSerializer.cpp
    Scene/SceneSerializer.h
    Scene/SceneComponents.h
    Scene/SpriteAnimation.cpp
    Scene/SpriteAnimation.h
    Scene/SpriteAnimationSet.cpp
    Scene/SpriteAnimationSet.h
    Scene/SpriteRenderer.cpp
    Scene/SpriteRenderer.h
    Object/Object.h
    Object/Actor.h
    Object/Player.cpp
    Object/Player.h
    Gameplay/PlayerMovementComponent.h
)

add_library(Scene STATIC ${SCENE_SRC})
target_include_directories(Scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Scene PUBLIC Renderer EngineDeps)

# --- Editor ---
set(EDITOR_SRC
    Editor/EditorLayer.cpp
    Editor/EditorLayer.h
    Editor/SceneEditorPanel.cpp
    Editor/SceneEditorPanel.h
)

add_library(Editor STATIC ${EDITOR_SRC})
target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Editor PUBLIC Scene EngineDeps)
