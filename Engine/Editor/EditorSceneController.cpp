#include "Editor/EditorSceneController.h"

#include <GLFW/glfw3.h>

#include "Editor/EditorLayer.h"
#include "Renderer/VulkanRenderer.h"
#include "Scene/Scene.h"

bool EditorSceneController::Update(
    Scene& scene,
    EditorLayer& editorLayer,
    const InputState& input,
    const SceneViewportState& viewportState,
    bool isPlaying,
    float deltaTime)
{
    CameraCommand command{};
    bool sceneReloaded = false;

    if (input.CanUseEditorViewportInput())
    {
        if (input.IsKeyDown(GLFW_KEY_A) || input.IsKeyDown(GLFW_KEY_LEFT))
            command.moveX -= 1.0f;
        if (input.IsKeyDown(GLFW_KEY_D) || input.IsKeyDown(GLFW_KEY_RIGHT))
            command.moveX += 1.0f;
        if (input.IsKeyDown(GLFW_KEY_W) || input.IsKeyDown(GLFW_KEY_UP))
            command.moveY -= 1.0f;
        if (input.IsKeyDown(GLFW_KEY_S) || input.IsKeyDown(GLFW_KEY_DOWN))
            command.moveY += 1.0f;

        if (input.IsKeyDown(GLFW_KEY_Q))
            command.zoomDelta -= 1.0f;
        if (input.IsKeyDown(GLFW_KEY_E))
            command.zoomDelta += 1.0f;
    }

    if (input.CanUseEditorShortcuts() && input.WasKeyPressed(GLFW_KEY_F10))
        m_ShowCollisionDebug = !m_ShowCollisionDebug;

    if (input.CanUseEditorShortcuts() && input.WasKeyPressed(GLFW_KEY_F5))
        scene.SaveToFile(m_SceneFilePath);

    if (input.CanUseEditorShortcuts() && input.WasKeyPressed(GLFW_KEY_F9))
        sceneReloaded = scene.LoadFromFile(m_SceneFilePath);

    if (viewportState.visible)
    {
        scene.UpdateCamera(
            command,
            deltaTime,
            static_cast<float>(viewportState.width),
            static_cast<float>(viewportState.height)
        );
    }

    (void)editorLayer;
    (void)isPlaying;
    return sceneReloaded;
}

void EditorSceneController::Render(Scene& scene, EditorLayer& editorLayer, VulkanRenderer& renderer) const
{
    const TileMapEditorPanel& tileMapPanel = editorLayer.GetTileMapEditorPanel();
    Entity selected = scene.GetEntity(tileMapPanel.GetSelectedObjectID());

    const bool tileMapOnly =
        tileMapPanel.IsTileMapViewEnabled() &&
        selected.IsValid() &&
        selected.HasTileMap();

    if (tileMapOnly)
        scene.RenderTileMapOnly(renderer, selected.GetID());
    else
        scene.Render(renderer);

    if (m_ShowCollisionDebug)
    {
        scene.RenderCollisionDebug(
            renderer,
            tileMapOnly ? selected.GetID() : 0,
            tileMapOnly
        );
    }
}
