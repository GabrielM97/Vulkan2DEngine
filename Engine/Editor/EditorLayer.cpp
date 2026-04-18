#include "Editor/EditorLayer.h"

#include <algorithm>
#include <imgui.h>

#include "Editor/SceneViewportPanel.h"
#include "Editor/SceneEditorPanel.h"
#include "Math/Transform2D.h"
#include "Math/TransformMath2D.h"
#include "Renderer/VulkanRenderer.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

EditorLayer::EditorLayer()
    : m_SceneViewportPanel(std::make_unique<SceneViewportPanel>())
    , m_SceneEditorPanel(std::make_unique<SceneEditorPanel>())
{
}

EditorLayer::~EditorLayer() = default;

void EditorLayer::Draw(Scene* scene, VulkanRenderer& renderer, ImTextureID sceneViewportTextureID, bool isPlaying)
{
    DrawDockspace();
    DrawTopBar(isPlaying);

    if (scene == nullptr)
        return;

    m_SceneViewportPanel->Draw(sceneViewportTextureID);
    m_SceneEditorPanel->Draw(*scene, m_SelectedObjectID);
    m_TileMapEditorPanel.Draw(*scene, renderer, m_SelectedObjectID);
    DrawSelectedObjectOverlay(*scene, isPlaying);
    DrawTileStampPreview(*scene, isPlaying);
}

const SceneViewportState& EditorLayer::GetSceneViewportState() const
{
    return m_SceneViewportPanel->GetState();
}

bool EditorLayer::ConsumePlayRequest()
{
    const bool requested = m_PlayRequested;
    m_PlayRequested = false;
    return requested;
}

bool EditorLayer::ConsumeStopRequest()
{
    const bool requested = m_StopRequested;
    m_StopRequested = false;
    return requested;
}

void EditorLayer::SetSelectedObjectID(Scene& scene, GameObjectID id)
{
    if (id != 0 && !scene.IsValidGameObject(id))
        id = 0;

    m_SelectedObjectID = id;
}

void EditorLayer::DrawDockspace()
{
#ifdef IMGUI_HAS_DOCK
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    const float topBarHeight = GetTopBarHeight();

    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + topBarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - topBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoNavFocus
        | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("EditorDockspace", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGui::DockSpace(ImGui::GetID("EditorDockspaceRoot"), ImVec2(0.0f, 0.0f), dockspaceFlags);
    ImGui::End();
#endif
}

void EditorLayer::DrawTopBar(bool isPlaying)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const float topBarHeight = GetTopBarHeight();
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoNavFocus;

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, topBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 8.0f));

    if (ImGui::Begin("EditorTopBar", nullptr, windowFlags))
    {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(isPlaying ? "Mode: Playing" : "Mode: Editing");
        ImGui::SameLine();

        if (!isPlaying)
        {
            if (ImGui::Button("Play"))
                m_PlayRequested = true;
        }
        else
        {
            if (ImGui::Button("Stop"))
                m_StopRequested = true;
        }
    }

    ImGui::End();
    ImGui::PopStyleVar(3);
}

bool EditorLayer::TryGetSelectionCorners(Entity entity, std::array<glm::vec2, 4>& outCorners) const
{
    if (!entity.IsValid() || !entity.IsActive())
        return false;

    const Transform2D transform = entity.GetTransform();

    if (entity.HasTileMap())
    {
        const glm::vec2 tileSize = entity.GetTileSize();
        const uint32_t mapWidth = entity.GetTileMapWidth();
        const uint32_t mapHeight = entity.GetTileMapHeight();

        if (tileSize.x <= 0.0f || tileSize.y <= 0.0f || mapWidth == 0 || mapHeight == 0)
            return false;

        const glm::vec2 size{
            static_cast<float>(mapWidth) * tileSize.x,
            static_cast<float>(mapHeight) * tileSize.y
        };

        outCorners = TransformMath2D::GetWorldCorners(transform, size);
        return true;
    }

    if (!entity.IsSpriteVisible())
        return false;

    const glm::vec2 size = entity.GetSpriteSize();
    if (size.x == 0.0f || size.y == 0.0f)
        return false;

    outCorners = TransformMath2D::GetWorldCorners(transform, size);
    return true;
}

void EditorLayer::DrawSelectedObjectOverlay(Scene& scene, bool isPlaying)
{
    if (isPlaying)
        return;

    if (!scene.IsValidGameObject(m_SelectedObjectID))
        return;

    const SceneViewportState& viewportState = m_SceneViewportPanel->GetState();
    if (!viewportState.visible)
        return;

    Entity selected = scene.GetEntity(m_SelectedObjectID);

    std::array<glm::vec2, 4> worldCorners{};
    if (!TryGetSelectionCorners(selected, worldCorners))
        return;

    const Camera2D& camera = scene.GetCamera();
    const ImVec2 viewportMin = viewportState.contentMin;
    std::array<ImVec2, 5> screenCorners{};
    ImVec2 center{0.0f, 0.0f};
    for (size_t i = 0; i < worldCorners.size(); ++i)
    {
        const glm::vec2 screenLocal = camera.WorldToScreen(
            worldCorners[i],
            static_cast<float>(viewportState.width),
            static_cast<float>(viewportState.height)
        );

        screenCorners[i] = {
            viewportMin.x + screenLocal.x,
            viewportMin.y + screenLocal.y
        };
        center.x += screenCorners[i].x;
        center.y += screenCorners[i].y;
    }

    screenCorners[4] = screenCorners[0];
    center.x /= static_cast<float>(worldCorners.size());
    center.y /= static_cast<float>(worldCorners.size());

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    drawList->PushClipRect(viewportState.contentMin, viewportState.contentMax, true);
    drawList->AddPolyline(screenCorners.data(), static_cast<int>(screenCorners.size()), IM_COL32(80, 160, 255, 255), 0, 2.0f);
    drawList->AddCircleFilled(center, 3.0f, IM_COL32(80, 160, 255, 255));
    drawList->PopClipRect();
}

void EditorLayer::DrawTileStampPreview(Scene& scene, bool isPlaying)
{
    if (isPlaying)
        return;

    const SceneViewportState& viewportState = m_SceneViewportPanel->GetState();
    if (!viewportState.visible || !viewportState.hovered)
        return;

    if (!scene.IsValidGameObject(m_TileMapEditorPanel.GetSelectedObjectID()))
        return;

    Entity selected = scene.GetEntity(m_TileMapEditorPanel.GetSelectedObjectID());
    if (!selected.HasTileMap())
        return;

    const glm::ivec2 hoveredTile = m_TileMapEditorPanel.GetHoveredTile();
    if (hoveredTile.x < 0 || hoveredTile.y < 0)
        return;

    const glm::ivec2 selectionSize = m_TileMapEditorPanel.GetSelectedAtlasSize();
    if (selectionSize.x <= 0 || selectionSize.y <= 0)
        return;

    const Transform2D mapTransform = selected.GetTransform();
    const glm::vec2 tileSize = selected.GetTileSize();
    const glm::vec2 worldMin = mapTransform.position + glm::vec2{
        static_cast<float>(hoveredTile.x) * tileSize.x,
        static_cast<float>(hoveredTile.y) * tileSize.y
    };
    const glm::vec2 worldMax = worldMin + glm::vec2{
        static_cast<float>(selectionSize.x) * tileSize.x,
        static_cast<float>(selectionSize.y) * tileSize.y
    };

    const Camera2D& camera = scene.GetCamera();
    const glm::vec2 screenMinLocal = camera.WorldToScreen(
        worldMin,
        static_cast<float>(viewportState.width),
        static_cast<float>(viewportState.height)
    );
    const glm::vec2 screenMaxLocal = camera.WorldToScreen(
        worldMax,
        static_cast<float>(viewportState.width),
        static_cast<float>(viewportState.height)
    );

    const ImVec2 viewportMin = viewportState.contentMin;
    const ImVec2 rectMin{
        viewportMin.x + std::min(screenMinLocal.x, screenMaxLocal.x),
        viewportMin.y + std::min(screenMinLocal.y, screenMaxLocal.y)
    };
    const ImVec2 rectMax{
        viewportMin.x + std::max(screenMinLocal.x, screenMaxLocal.x),
        viewportMin.y + std::max(screenMinLocal.y, screenMaxLocal.y)
    };

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    drawList->AddRectFilled(rectMin, rectMax, IM_COL32(255, 200, 0, 40));
    drawList->AddRect(rectMin, rectMax, IM_COL32(255, 200, 0, 220), 0.0f, 0, 2.0f);
}
