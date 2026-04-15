#include "Editor/TileMapEditorPanel.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>

#include <imgui.h>

#include "Scene/Scene.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <commdlg.h>
#endif

namespace
{
#ifdef _WIN32
    std::string OpenTilesetTexturePath()
    {
        char filePath[MAX_PATH] = {};

        OPENFILENAMEA dialog{};
        dialog.lStructSize = sizeof(dialog);
        dialog.lpstrFilter =
            "Image Files\0*.png;*.jpg;*.jpeg;*.bmp;*.tga\0"
            "All Files\0*.*\0";
        dialog.lpstrFile = filePath;
        dialog.nMaxFile = MAX_PATH;
        dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        dialog.lpstrTitle = "Select Tileset Texture";

        if (!GetOpenFileNameA(&dialog))
            return {};

        std::filesystem::path selectedPath = std::filesystem::path(filePath);
        std::error_code errorCode;
        const std::filesystem::path relativePath =
            std::filesystem::relative(selectedPath, std::filesystem::current_path(), errorCode);

        if (errorCode)
            return selectedPath.generic_string();

        return relativePath.generic_string();
    }
#endif
}

void TileMapEditorPanel::SyncFromSelection(Scene& scene, GameObjectID selectedObjectID)
{
    m_SelectedObjectID = selectedObjectID;

    if (!scene.IsValidGameObject(m_SelectedObjectID))
    {
        m_TileMapTexturePathBuffer[0] = '\0';
        m_TileMapWidthDraft = 16;
        m_TileMapHeightDraft = 16;
        m_TileMapColumnsDraft = 1;
        m_TileMapRowsDraft = 1;
        m_SelectedTileID = 0;
        m_TileEditX = 0;
        m_TileEditY = 0;
        m_TileEditValue = 0;
        return;
    }

    Entity entity = scene.GetEntity(m_SelectedObjectID);
    if (!entity.HasTileMap())
    {
        m_TileMapTexturePathBuffer[0] = '\0';
        m_TileMapWidthDraft = 16;
        m_TileMapHeightDraft = 16;
        m_TileMapColumnsDraft = 1;
        m_TileMapRowsDraft = 1;
        m_TileEditX = 0;
        m_TileEditY = 0;
        m_TileEditValue = 0;
        return;
    }

    std::snprintf(
        m_TileMapTexturePathBuffer.data(),
        m_TileMapTexturePathBuffer.size(),
        "%s",
        entity.GetTileMapTexturePath().c_str()
    );

    m_TileMapWidthDraft = static_cast<int>(entity.GetTileMapWidth());
    m_TileMapHeightDraft = static_cast<int>(entity.GetTileMapHeight());
    m_TileMapColumnsDraft = static_cast<int>(entity.GetTileMapColumns());
    m_TileMapRowsDraft = static_cast<int>(entity.GetTileMapRows());

    if (m_TileEditX >= m_TileMapWidthDraft)
        m_TileEditX = 0;
    if (m_TileEditY >= m_TileMapHeightDraft)
        m_TileEditY = 0;

    m_TileEditValue = entity.GetTile(m_TileEditX, m_TileEditY);
}

bool TileMapEditorPanel::HasActiveTileMap(Scene& scene) const
{
    if (!scene.IsValidGameObject(m_SelectedObjectID))
        return false;

    return scene.GetEntity(m_SelectedObjectID).HasTileMap();
}

void TileMapEditorPanel::Draw(Scene& scene, GameObjectID selectedObjectID)
{
    ImGui::Begin("Tile Map Editor");

    if (m_SelectedObjectID != selectedObjectID)
        SyncFromSelection(scene, selectedObjectID);

    if (!scene.IsValidGameObject(selectedObjectID))
    {
        ImGui::TextUnformatted("No object selected.");
        ImGui::End();
        return;
    }

    Entity entity = scene.GetEntity(selectedObjectID);

    ImGui::Text("Selected Object: %s", entity.GetName().c_str());
    ImGui::Text("ID: %llu", static_cast<unsigned long long>(selectedObjectID));
    ImGui::Separator();

    if (!entity.HasTileMap())
    {
        ImGui::TextDisabled("Selected object has no tile map.");

        if (ImGui::Button("Add Tile Map"))
        {
            entity.EnsureTileMap();
            entity.ResizeTileMap(
                static_cast<uint32_t>(std::max(1, m_TileMapWidthDraft)),
                static_cast<uint32_t>(std::max(1, m_TileMapHeightDraft))
            );
            entity.SetTileMapGrid(
                static_cast<uint32_t>(std::max(1, m_TileMapColumnsDraft)),
                static_cast<uint32_t>(std::max(1, m_TileMapRowsDraft))
            );
            entity.SetTileMapTexturePath(m_TileMapTexturePathBuffer.data());
            SyncFromSelection(scene, selectedObjectID);
        }

        ImGui::End();
        return;
    }

    if (ImGui::Button("Remove Tile Map"))
    {
        entity.RemoveTileMap();
        SyncFromSelection(scene, selectedObjectID);
        ImGui::End();
        return;
    }

    ImGui::SeparatorText("Map");

    if (ImGui::InputText(
            "Tileset Texture Path",
            m_TileMapTexturePathBuffer.data(),
            m_TileMapTexturePathBuffer.size()))
    {
        entity.SetTileMapTexturePath(m_TileMapTexturePathBuffer.data());
    }

#ifdef _WIN32
    ImGui::SameLine();
    if (ImGui::Button("Browse..."))
    {
        const std::string pickedPath = OpenTilesetTexturePath();
        if (!pickedPath.empty())
        {
            std::snprintf(
                m_TileMapTexturePathBuffer.data(),
                m_TileMapTexturePathBuffer.size(),
                "%s",
                pickedPath.c_str()
            );
            entity.SetTileMapTexturePath(m_TileMapTexturePathBuffer.data());
        }
    }
#endif

    glm::vec2 tileSize = entity.GetTileSize();
    if (ImGui::DragFloat2("Tile Size", &tileSize.x, 1.0f, 1.0f, 4096.0f))
        entity.SetTileSize(tileSize);

    ImGui::InputInt("Map Width", &m_TileMapWidthDraft);
    ImGui::InputInt("Map Height", &m_TileMapHeightDraft);

    if (ImGui::Button("Resize Tile Map"))
    {
        m_TileMapWidthDraft = std::max(1, m_TileMapWidthDraft);
        m_TileMapHeightDraft = std::max(1, m_TileMapHeightDraft);
        entity.ResizeTileMap(
            static_cast<uint32_t>(m_TileMapWidthDraft),
            static_cast<uint32_t>(m_TileMapHeightDraft)
        );
    }

    ImGui::SeparatorText("Atlas");

    ImGui::InputInt("Columns", &m_TileMapColumnsDraft);
    ImGui::InputInt("Rows", &m_TileMapRowsDraft);

    if (ImGui::Button("Apply Grid"))
    {
        m_TileMapColumnsDraft = std::max(1, m_TileMapColumnsDraft);
        m_TileMapRowsDraft = std::max(1, m_TileMapRowsDraft);
        entity.SetTileMapGrid(
            static_cast<uint32_t>(m_TileMapColumnsDraft),
            static_cast<uint32_t>(m_TileMapRowsDraft)
        );
    }

    ImGui::SeparatorText("Brush");

    ImGui::Checkbox("Paint Mode", &m_PaintModeEnabled);
    ImGui::SameLine();
    ImGui::Checkbox("Erase Mode", &m_EraseModeEnabled);

    if (m_EraseModeEnabled)
        m_PaintModeEnabled = false;

    if (m_PaintModeEnabled)
        m_EraseModeEnabled = false;

    ImGui::InputInt("Selected Tile ID", &m_SelectedTileID);
    if (m_SelectedTileID < -1)
        m_SelectedTileID = -1;

    ImGui::Text("Hovered Tile: (%d, %d)", m_HoveredTile.x, m_HoveredTile.y);
    ImGui::End();
}
