#include "Editor/EditorSceneController.h"

#include <limits>

#include <GLFW/glfw3.h>

#include "Editor/EditorLayer.h"
#include "Editor/EditorViewportMath.h"
#include "Math/TransformMath2D.h"
#include "Renderer/VulkanRenderer.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

bool EditorSceneController::TryGetSelectionShape(Entity entity, Transform2D& outTransform, glm::vec2& outSize) const
{
    if (!entity.IsValid() || !entity.IsActive())
        return false;

    outTransform = entity.GetTransform();

    if (entity.HasTileMap())
    {
        const glm::vec2 tileSize = entity.GetTileSize();
        const uint32_t mapWidth = entity.GetTileMapWidth();
        const uint32_t mapHeight = entity.GetTileMapHeight();

        if (tileSize.x <= 0.0f || tileSize.y <= 0.0f || mapWidth == 0 || mapHeight == 0)
            return false;

        outSize = {
            static_cast<float>(mapWidth) * tileSize.x,
            static_cast<float>(mapHeight) * tileSize.y
        };
        return true;
    }

    if (!entity.IsSpriteVisible())
        return false;

    const glm::vec2 size = entity.GetSpriteSize();
    if (size.x == 0.0f || size.y == 0.0f)
        return false;

    outSize = size;
    return true;
}

bool EditorSceneController::TryPickGameObject(
    Scene& scene,
    const InputState& input,
    const SceneViewportState& viewportState,
    GameObjectID& outObjectID) const
{
    if (!viewportState.visible)
        return false;

    const glm::vec2 mouseScreen = input.GetMouseScreenPosition();
    const glm::vec2 worldPosition = EditorViewportMath::ScreenToWorld(scene, mouseScreen, viewportState);

    bool found = false;
    GameObjectID bestID = 0;
    int bestLayer = std::numeric_limits<int>::min();

    for (GameObjectID id : scene.GetGameObjectIDs())
    {
        Entity entity = scene.GetEntity(id);
        Transform2D pickTransform{};
        glm::vec2 pickSize{};
        if (!TryGetSelectionShape(entity, pickTransform, pickSize))
            continue;

        if (!TransformMath2D::ContainsWorldPoint(worldPosition, pickTransform, pickSize))
            continue;

        const int layer = entity.GetSpriteLayer();
        if (!found || layer > bestLayer)
        {
            found = true;
            bestID = id;
            bestLayer = layer;
        }
    }

    if (!found)
        return false;

    outObjectID = bestID;
    return true;
}

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

    if (!isPlaying && input.CanUseEditorViewportInput() && input.WasMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        GameObjectID pickedID = 0;
        if (TryPickGameObject(scene, input, viewportState, pickedID))
            editorLayer.SetSelectedObjectID(scene, pickedID);
        else
            editorLayer.SetSelectedObjectID(scene, 0);
    }

    if (viewportState.visible)
    {
        scene.UpdateCamera(
            command,
            deltaTime,
            static_cast<float>(viewportState.width),
            static_cast<float>(viewportState.height)
        );
    }

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
