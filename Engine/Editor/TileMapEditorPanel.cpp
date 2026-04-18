#include "Editor/TileMapEditorPanel.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>

#include <imgui.h>
#include <stb_image.h>

#include "Renderer/VulkanRenderer.h"
#include "Scene/Scene.h"
#include "Scene/TileSetAsset.h"
#include "Scene/TileMapAssetSerializer.h"

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
    bool TryGetImageSize(const std::string& path, int& outWidth, int& outHeight)
    {
        int channels = 0;
        return stbi_info(path.c_str(), &outWidth, &outHeight, &channels) != 0;
    }

    bool TryLoadTileSetAsset(
        const std::string& assetPath,
        TileSetAsset& outAsset,
        int& outCellWidth,
        int& outCellHeight)
    {
        if (assetPath.empty() || !outAsset.LoadFromFile(assetPath))
            return false;

        outCellWidth = 32;
        outCellHeight = 32;

        int textureWidth = 0;
        int textureHeight = 0;
        if (TryGetImageSize(outAsset.GetTexturePath(), textureWidth, textureHeight))
        {
            outCellWidth = std::max(1, textureWidth / std::max(1, static_cast<int>(outAsset.GetColumns())));
            outCellHeight = std::max(1, textureHeight / std::max(1, static_cast<int>(outAsset.GetRows())));
        }

        return true;
    }

#ifdef _WIN32
    std::string OpenTileMapAssetPath()
    {
        char filePath[MAX_PATH] = {};

        std::filesystem::path basePath = std::filesystem::current_path();

        OPENFILENAMEA dialog{};
        dialog.lStructSize = sizeof(dialog);
        dialog.lpstrFilter =
            "Tile Map Assets\0*.tilemap.json\0"
            "JSON Files\0*.json\0"
            "All Files\0*.*\0";
        dialog.lpstrFile = filePath;
        dialog.nMaxFile = MAX_PATH;
        dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        dialog.lpstrTitle = "Open Tile Map Asset";

        if (!GetOpenFileNameA(&dialog))
            return {};

        std::filesystem::path selectedPath = std::filesystem::path(filePath);
        std::error_code errorCode;
        const std::filesystem::path relativePath =
            std::filesystem::relative(selectedPath, basePath, errorCode);

        std::filesystem::current_path(basePath);

        if (errorCode)
            return selectedPath.generic_string();

        return relativePath.generic_string();
    }

    std::string OpenTileSetAssetPath()
    {
        char filePath[MAX_PATH] = {};

        std::filesystem::path basePath = std::filesystem::current_path();

        OPENFILENAMEA dialog{};
        dialog.lStructSize = sizeof(dialog);
        dialog.lpstrFilter =
            "Tile Set Assets\0*.tileset.json\0"
            "JSON Files\0*.json\0"
            "All Files\0*.*\0";
        dialog.lpstrFile = filePath;
        dialog.nMaxFile = MAX_PATH;
        dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
        dialog.lpstrTitle = "Open Tile Set Asset";

        if (!GetOpenFileNameA(&dialog))
            return {};

        std::filesystem::path selectedPath = std::filesystem::path(filePath);
        std::error_code errorCode;
        const std::filesystem::path relativePath =
            std::filesystem::relative(selectedPath, basePath, errorCode);

        std::filesystem::current_path(basePath);

        if (errorCode)
            return selectedPath.generic_string();

        return relativePath.generic_string();
    }

    std::string SaveTileMapAssetPath()
    {
        char filePath[MAX_PATH] = {};

        std::filesystem::path basePath = std::filesystem::current_path();

        OPENFILENAMEA dialog{};
        dialog.lStructSize = sizeof(dialog);
        dialog.lpstrFilter =
            "Tile Map Assets\0*.tilemap.json\0"
            "JSON Files\0*.json\0"
            "All Files\0*.*\0";
        dialog.lpstrFile = filePath;
        dialog.nMaxFile = MAX_PATH;
        dialog.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
        dialog.lpstrDefExt = "json";
        dialog.lpstrTitle = "Save Tile Map Asset";

        if (!GetSaveFileNameA(&dialog))
            return {};

        std::filesystem::path selectedPath = std::filesystem::path(filePath);
        if (selectedPath.extension() != ".json" || selectedPath.stem().extension() != ".tilemap")
            selectedPath += ".tilemap.json";

        std::error_code errorCode;
        const std::filesystem::path relativePath =
            std::filesystem::relative(selectedPath, basePath, errorCode);

        std::filesystem::current_path(basePath);

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
        m_TileSetAssetPathBuffer[0] = '\0';
        m_TileMapAssetPathBuffer[0] = '\0';
        m_TileMapWidthDraft = 16;
        m_TileMapHeightDraft = 16;
        m_TileMapColumnsDraft = 1;
        m_TileMapRowsDraft = 1;
        m_AtlasCellWidthDraft = 32;
        m_AtlasCellHeightDraft = 32;
        m_SelectedTileID = 0;
        m_TileMapViewEnabled = false;
        m_AtlasSelectionStart = {0, 0};
        m_AtlasSelectionEnd = {0, 0};
        m_TileEditX = 0;
        m_TileEditY = 0;
        m_TileEditValue = 0;
        return;
    }

    Entity entity = scene.GetEntity(m_SelectedObjectID);
    if (!entity.HasTileMap())
    {
        m_TileMapTexturePathBuffer[0] = '\0';
        m_TileSetAssetPathBuffer[0] = '\0';
        m_TileMapAssetPathBuffer[0] = '\0';
        m_TileMapWidthDraft = 16;
        m_TileMapHeightDraft = 16;
        m_TileMapColumnsDraft = 1;
        m_TileMapRowsDraft = 1;
        m_AtlasCellWidthDraft = 32;
        m_AtlasCellHeightDraft = 32;
        m_SelectedTileID = 0;
        m_TileMapViewEnabled = false;
        m_AtlasSelectionStart = {0, 0};
        m_AtlasSelectionEnd = {0, 0};
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
    std::snprintf(
        m_TileSetAssetPathBuffer.data(),
        m_TileSetAssetPathBuffer.size(),
        "%s",
        entity.GetTileSetAssetPath().c_str()
    );
    std::snprintf(
        m_TileMapAssetPathBuffer.data(),
        m_TileMapAssetPathBuffer.size(),
        "%s",
        entity.GetTileMapAssetPath().c_str()
    );

    m_TileMapWidthDraft = static_cast<int>(entity.GetTileMapWidth());
    m_TileMapHeightDraft = static_cast<int>(entity.GetTileMapHeight());
    m_TileMapColumnsDraft = static_cast<int>(entity.GetTileMapColumns());
    m_TileMapRowsDraft = static_cast<int>(entity.GetTileMapRows());
    m_ActiveLayerDraft = static_cast<int>(entity.GetActiveTileLayerIndex());
    const glm::ivec2 atlasCellSize = entity.GetTileAtlasCellSize();
    m_AtlasCellWidthDraft = atlasCellSize.x;
    m_AtlasCellHeightDraft = atlasCellSize.y;

    TileSetAsset tileSetAsset;
    int resolvedCellWidth = m_AtlasCellWidthDraft;
    int resolvedCellHeight = m_AtlasCellHeightDraft;
    if (TryLoadTileSetAsset(entity.GetTileSetAssetPath(), tileSetAsset, resolvedCellWidth, resolvedCellHeight))
    {
        std::snprintf(
            m_TileMapTexturePathBuffer.data(),
            m_TileMapTexturePathBuffer.size(),
            "%s",
            tileSetAsset.GetTexturePath().c_str()
        );
        m_TileMapColumnsDraft = static_cast<int>(tileSetAsset.GetColumns());
        m_TileMapRowsDraft = static_cast<int>(tileSetAsset.GetRows());
        m_AtlasCellWidthDraft = resolvedCellWidth;
        m_AtlasCellHeightDraft = resolvedCellHeight;
    }

    m_AtlasSelectionStart = {
        m_SelectedTileID % std::max(1, m_TileMapColumnsDraft),
        m_SelectedTileID / std::max(1, m_TileMapColumnsDraft)
    };
    m_AtlasSelectionEnd = m_AtlasSelectionStart;

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

glm::ivec2 TileMapEditorPanel::GetSelectedAtlasOrigin() const
{
    return {
        std::min(m_AtlasSelectionStart.x, m_AtlasSelectionEnd.x),
        std::min(m_AtlasSelectionStart.y, m_AtlasSelectionEnd.y)
    };
}

glm::ivec2 TileMapEditorPanel::GetSelectedAtlasSize() const
{
    const glm::ivec2 origin = GetSelectedAtlasOrigin();
    const glm::ivec2 maxCorner{
        std::max(m_AtlasSelectionStart.x, m_AtlasSelectionEnd.x),
        std::max(m_AtlasSelectionStart.y, m_AtlasSelectionEnd.y)
    };

    return {
        maxCorner.x - origin.x + 1,
        maxCorner.y - origin.y + 1
    };
}

int TileMapEditorPanel::GetSelectedTileIDAtOffset(glm::ivec2 offset, int atlasColumns) const
{
    const glm::ivec2 origin = GetSelectedAtlasOrigin();
    return (origin.y + offset.y) * atlasColumns + (origin.x + offset.x);
}

void TileMapEditorPanel::DrawAtlasPicker(Entity& entity, VulkanRenderer& renderer)
{
    std::string texturePath = entity.GetTileMapTexturePath();
    TileSetAsset tileSetAsset;
    int ignoredCellWidth = 0;
    int ignoredCellHeight = 0;
    if (TryLoadTileSetAsset(entity.GetTileSetAssetPath(), tileSetAsset, ignoredCellWidth, ignoredCellHeight))
        texturePath = tileSetAsset.GetTexturePath();

    if (texturePath.empty())
    {
        ImGui::TextDisabled("No tileset asset or texture selected.");
        return;
    }

    const uint32_t textureIndex = renderer.GetOrLoadTexture(texturePath);
    const VulkanTexture* texture = renderer.GetTexture(textureIndex);
    ImTextureID textureID = renderer.GetOrCreateImGuiTextureID(texturePath);

    if (texture == nullptr || textureID == NULL || texture->GetWidth() == 0 || texture->GetHeight() == 0)
    {
        ImGui::TextDisabled("Failed to load tileset preview.");
        return;
    }

    const uint32_t columns = std::max<uint32_t>(1, entity.GetTileMapColumns());
    const uint32_t rows = std::max<uint32_t>(1, entity.GetTileMapRows());

    ImGui::SliderFloat("Preview Scale", &m_AtlasPreviewScale, 0.25f, 8.0f, "%.2fx");

    const ImVec2 imageSize{
        static_cast<float>(texture->GetWidth()) * m_AtlasPreviewScale,
        static_cast<float>(texture->GetHeight()) * m_AtlasPreviewScale
    };

    ImGui::BeginChild("TileAtlasPreview", ImVec2(0.0f, 320.0f), true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Image(textureID, imageSize);

    const ImVec2 imageMin = ImGui::GetItemRectMin();
    const ImVec2 imageMax = ImGui::GetItemRectMax();
    const float cellWidth = (imageMax.x - imageMin.x) / static_cast<float>(columns);
    const float cellHeight = (imageMax.y - imageMin.y) / static_cast<float>(rows);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    for (uint32_t x = 1; x < columns; ++x)
    {
        const float px = imageMin.x + cellWidth * static_cast<float>(x);
        drawList->AddLine(ImVec2(px, imageMin.y), ImVec2(px, imageMax.y), IM_COL32(255, 255, 255, 80));
    }

    for (uint32_t y = 1; y < rows; ++y)
    {
        const float py = imageMin.y + cellHeight * static_cast<float>(y);
        drawList->AddLine(ImVec2(imageMin.x, py), ImVec2(imageMax.x, py), IM_COL32(255, 255, 255, 80));
    }

    const glm::ivec2 selectedOrigin = GetSelectedAtlasOrigin();
    const glm::ivec2 selectedSize = GetSelectedAtlasSize();
    if (m_SelectedTileID >= 0)
    {
        if (selectedOrigin.x < static_cast<int>(columns) &&
            selectedOrigin.y < static_cast<int>(rows))
        {
            const ImVec2 selectionMin{
                imageMin.x + cellWidth * static_cast<float>(selectedOrigin.x),
                imageMin.y + cellHeight * static_cast<float>(selectedOrigin.y)
            };
            const ImVec2 selectionMax{
                selectionMin.x + cellWidth * static_cast<float>(selectedSize.x),
                selectionMin.y + cellHeight * static_cast<float>(selectedSize.y)
            };

            drawList->AddRect(selectionMin, selectionMax, IM_COL32(255, 200, 0, 255), 0.0f, 0, 3.0f);
        }
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        const ImVec2 mouse = ImGui::GetMousePos();
        const int column = static_cast<int>((mouse.x - imageMin.x) / cellWidth);
        const int row = static_cast<int>((mouse.y - imageMin.y) / cellHeight);

        if (column >= 0 && column < static_cast<int>(columns) &&
            row >= 0 && row < static_cast<int>(rows))
        {
            m_IsAtlasSelecting = true;
            m_AtlasSelectionStart = {column, row};
            m_AtlasSelectionEnd = m_AtlasSelectionStart;
            m_SelectedTileID = row * static_cast<int>(columns) + column;
        }
    }

    if (m_IsAtlasSelecting && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        const ImVec2 mouse = ImGui::GetMousePos();
        const int column = std::clamp(
            static_cast<int>((mouse.x - imageMin.x) / cellWidth),
            0,
            static_cast<int>(columns) - 1
        );
        const int row = std::clamp(
            static_cast<int>((mouse.y - imageMin.y) / cellHeight),
            0,
            static_cast<int>(rows) - 1
        );

        m_AtlasSelectionEnd = {column, row};
        const glm::ivec2 origin = GetSelectedAtlasOrigin();
        m_SelectedTileID = origin.y * static_cast<int>(columns) + origin.x;
    }

    if (m_IsAtlasSelecting && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        m_IsAtlasSelecting = false;
        const glm::ivec2 origin = GetSelectedAtlasOrigin();
        m_SelectedTileID = origin.y * static_cast<int>(columns) + origin.x;
    }

    ImGui::EndChild();
}

void TileMapEditorPanel::ApplyAtlusCellSize(Entity entity)
{
    m_AtlasCellWidthDraft = std::max(1, m_AtlasCellWidthDraft);
    m_AtlasCellHeightDraft = std::max(1, m_AtlasCellHeightDraft);

    entity.SetTileAtlasCellSize({
        m_AtlasCellWidthDraft,
        m_AtlasCellHeightDraft
    });

    int textureWidth = 0;
    int textureHeight = 0;
    if (TryGetImageSize(entity.GetTileMapTexturePath(), textureWidth, textureHeight))
    {
        m_TileMapColumnsDraft = std::max(1, textureWidth / m_AtlasCellWidthDraft);
        m_TileMapRowsDraft = std::max(1, textureHeight / m_AtlasCellHeightDraft);
        entity.SetTileMapGrid(
            static_cast<uint32_t>(m_TileMapColumnsDraft),
            static_cast<uint32_t>(m_TileMapRowsDraft)
        );
    }
}

bool TileMapEditorPanel::ApplyTileSetAsset(Entity entity)
{
    TileSetAsset tileSetAsset;
    int resolvedCellWidth = 32;
    int resolvedCellHeight = 32;
    if (!TryLoadTileSetAsset(m_TileSetAssetPathBuffer.data(), tileSetAsset, resolvedCellWidth, resolvedCellHeight))
        return false;

    entity.SetTileSetAssetPath(m_TileSetAssetPathBuffer.data());
    entity.SetTileMapTexturePath(tileSetAsset.GetTexturePath());
    entity.SetTileMapGrid(tileSetAsset.GetColumns(), tileSetAsset.GetRows());
    entity.SetTileAtlasCellSize({resolvedCellWidth, resolvedCellHeight});

    std::snprintf(
        m_TileMapTexturePathBuffer.data(),
        m_TileMapTexturePathBuffer.size(),
        "%s",
        tileSetAsset.GetTexturePath().c_str()
    );
    m_TileMapColumnsDraft = static_cast<int>(tileSetAsset.GetColumns());
    m_TileMapRowsDraft = static_cast<int>(tileSetAsset.GetRows());
    m_AtlasCellWidthDraft = resolvedCellWidth;
    m_AtlasCellHeightDraft = resolvedCellHeight;
    return true;
}

void TileMapEditorPanel::DrawLayerControls(Entity entity)
{
    ImGui::SeparatorText("Layers");

    if (ImGui::Button("Add Layer"))
        entity.AddTileLayer(std::string("Layer ") + std::to_string(entity.GetTileLayerCount()));

    ImGui::SameLine();
    if (ImGui::Button("Remove Active Layer"))
        entity.RemoveTileLayer(entity.GetActiveTileLayerIndex());

    const uint32_t layerCount = entity.GetTileLayerCount();
    const uint32_t activeLayer = entity.GetActiveTileLayerIndex();
    m_ActiveLayerDraft = static_cast<int>(activeLayer);

    ImGui::TextUnformatted("A = Active, V = Visible, C = Collision");
    
    for (uint32_t layerIndex = 0; layerIndex < layerCount; ++layerIndex)
    {
        bool active = layerIndex == activeLayer;
        if (ImGui::RadioButton(("##LayerActive" + std::to_string(layerIndex)).c_str(), active))
            entity.SetActiveTileLayerIndex(layerIndex);

        ImGui::SameLine();

        bool visible = entity.IsTileLayerVisible(layerIndex);
        if (ImGui::Checkbox(("##LayerVisible" + std::to_string(layerIndex)).c_str(), &visible))
            entity.SetTileLayerVisible(layerIndex, visible);

        ImGui::SameLine();

        bool collisionEnabled = entity.IsTileLayerCollisionEnabled(layerIndex);
        if (ImGui::Checkbox(("##LayerCollision" + std::to_string(layerIndex)).c_str(), &collisionEnabled))
            entity.SetTileLayerCollisionEnabled(layerIndex, collisionEnabled);

        ImGui::SameLine();
        ImGui::Text("%s", entity.GetTileLayerName(layerIndex).c_str());
    }
}

void TileMapEditorPanel::DrawAssetControls(Scene& scene, Entity entity)
{
    ImGui::SeparatorText("Tile Map Asset");

    ImGui::Text(
        "Backed By Asset: %s",
        entity.IsTileMapAssetBacked() ? "Yes" : "No (scene-local)"
    );

    ImGui::InputText(
        "Asset Path",
        m_TileMapAssetPathBuffer.data(),
        m_TileMapAssetPathBuffer.size()
    );

#ifdef _WIN32
    if (ImGui::Button("Browse Asset..."))
    {
        const std::string pickedPath = OpenTileMapAssetPath();
        if (!pickedPath.empty())
        {
            std::snprintf(
                m_TileMapAssetPathBuffer.data(),
                m_TileMapAssetPathBuffer.size(),
                "%s",
                pickedPath.c_str()
            );
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Save As New Asset..."))
    {
        const std::string pickedPath = SaveTileMapAssetPath();
        if (!pickedPath.empty())
        {
            std::snprintf(
                m_TileMapAssetPathBuffer.data(),
                m_TileMapAssetPathBuffer.size(),
                "%s",
                pickedPath.c_str()
            );

            if (TileMapAssetSerializer::SaveToFile(entity, pickedPath))
            {
                entity.SetTileMapAssetPath(pickedPath);
                SyncFromSelection(scene, entity.GetID());
            }
        }
    }
#endif

    if (ImGui::Button("Load From Asset"))
    {
        if (m_TileMapAssetPathBuffer[0] != '\0' &&
            TileMapAssetSerializer::LoadIntoEntity(m_TileMapAssetPathBuffer.data(), entity))
        {
            m_SelectedTileID = 0;
            SyncFromSelection(scene, entity.GetID());
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Reload From Asset"))
    {
        const std::string assetPath = entity.GetTileMapAssetPath();
        if (!assetPath.empty() && TileMapAssetSerializer::LoadIntoEntity(assetPath, entity))
        {
            m_SelectedTileID = 0;
            SyncFromSelection(scene, entity.GetID());
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Save Back To Asset"))
    {
        const std::string assetPath = entity.GetTileMapAssetPath();
        if (!assetPath.empty())
        {
            if (TileMapAssetSerializer::SaveToFile(entity, assetPath))
                SyncFromSelection(scene, entity.GetID());
        }
    }
}

void TileMapEditorPanel::Draw(Scene& scene, VulkanRenderer& renderer, GameObjectID selectedObjectID)
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
            if (!ApplyTileSetAsset(entity))
            {
                entity.SetTileMapGrid(
                    static_cast<uint32_t>(std::max(1, m_TileMapColumnsDraft)),
                    static_cast<uint32_t>(std::max(1, m_TileMapRowsDraft))
                );
                entity.SetTileMapTexturePath(m_TileMapTexturePathBuffer.data());
            }
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

    ImGui::Checkbox("Tile Map View", &m_TileMapViewEnabled);
    ImGui::Separator();

    ImGui::SeparatorText("Map");

    if (ImGui::InputText(
            "Tileset Asset Path",
            m_TileSetAssetPathBuffer.data(),
            m_TileSetAssetPathBuffer.size()))
    {
        entity.SetTileSetAssetPath(m_TileSetAssetPathBuffer.data());
        ApplyTileSetAsset(entity);
    }

#ifdef _WIN32
    ImGui::SameLine();
    if (ImGui::Button("Browse Tileset Asset..."))
    {
        const std::string pickedPath = OpenTileSetAssetPath();
        if (!pickedPath.empty())
        {
            std::snprintf(
                m_TileSetAssetPathBuffer.data(),
                m_TileSetAssetPathBuffer.size(),
                "%s",
                pickedPath.c_str()
            );
            ApplyTileSetAsset(entity);
        }
    }
#endif

    if (m_TileMapTexturePathBuffer[0] != '\0')
        ImGui::Text("Resolved Texture: %s", m_TileMapTexturePathBuffer.data());

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
    
    const glm::ivec2 atlasCellSize = entity.GetTileAtlasCellSize();
    ImGui::Text("Atlas Cell Size: %d x %d px", atlasCellSize.x, atlasCellSize.y);

    ImGui::InputInt("Atlas Cell Width", &m_AtlasCellWidthDraft);
    ImGui::InputInt("Atlas Cell Height", &m_AtlasCellHeightDraft);

    if (ImGui::Button("Apply Cell Size"))
    {
        ApplyAtlusCellSize(entity);
    }

    int textureWidth = 0;
    int textureHeight = 0;
    if (TryGetImageSize(m_TileMapTexturePathBuffer.data(), textureWidth, textureHeight))
    {
        const bool widthEven = (textureWidth % std::max(1, m_AtlasCellWidthDraft)) == 0;
        const bool heightEven = (textureHeight % std::max(1, m_AtlasCellHeightDraft)) == 0;
        if (!widthEven || !heightEven)
            ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.2f, 1.0f), "Warning: atlas cell size does not evenly divide the texture.");
    }

    ImGui::SeparatorText("Atlas Picker");
    DrawAtlasPicker(entity, renderer);
    DrawLayerControls(entity);
    DrawAssetControls(scene, entity);

    ImGui::SeparatorText("Brush");

    ImGui::Checkbox("Paint Mode", &m_PaintModeEnabled);
    ImGui::SameLine();
    ImGui::Checkbox("Erase Mode", &m_EraseModeEnabled);
    ImGui::SameLine();
    ImGui::Checkbox("Fill Mode", &m_FillModeEnabled);

    if (m_EraseModeEnabled)
    {
        m_PaintModeEnabled = false;
        m_FillModeEnabled = false;
    }

    if (m_PaintModeEnabled || m_FillModeEnabled)
        m_EraseModeEnabled = false;

    if (m_FillModeEnabled)
        m_PaintModeEnabled = false;

    if (ImGui::InputInt("Selected Tile ID", &m_SelectedTileID))
    {
        m_SelectedTileID = std::max(-1, m_SelectedTileID);
        if (m_SelectedTileID >= 0)
        {
            const int atlasColumns = std::max(1, static_cast<int>(entity.GetTileMapColumns()));
            const int atlasRows = std::max(1, static_cast<int>(entity.GetTileMapRows()));
            const int maxTileID = atlasColumns * atlasRows - 1;
            m_SelectedTileID = std::clamp(m_SelectedTileID, 0, maxTileID);

            const glm::ivec2 selectionCell{
                m_SelectedTileID % atlasColumns,
                m_SelectedTileID / atlasColumns
            };
            m_AtlasSelectionStart = selectionCell;
            m_AtlasSelectionEnd = selectionCell;
        }
    }
    if (m_SelectedTileID < -1)
        m_SelectedTileID = -1;

    const glm::ivec2 atlasSelectionOrigin = GetSelectedAtlasOrigin();
    const glm::ivec2 atlasSelectionSize = GetSelectedAtlasSize();
    ImGui::Text(
        "Atlas Selection: (%d, %d) size %d x %d",
        atlasSelectionOrigin.x,
        atlasSelectionOrigin.y,
        atlasSelectionSize.x,
        atlasSelectionSize.y
    );
    ImGui::Text("Hovered Tile: (%d, %d)", m_HoveredTile.x, m_HoveredTile.y);
    ImGui::End();
}
