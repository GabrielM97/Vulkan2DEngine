#include "Editor/TileSetEditorPanel.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>

#include <imgui.h>
#include <stb_image.h>

#include "Renderer/VulkanRenderer.h"

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

#ifdef _WIN32
    std::string ToProjectRelativePath(const std::filesystem::path& selectedPath)
    {
        const std::filesystem::path basePath = std::filesystem::current_path();

        std::error_code errorCode;
        const std::filesystem::path relativePath =
            std::filesystem::relative(selectedPath, basePath, errorCode);

        std::filesystem::current_path(basePath);

        if (errorCode)
            return selectedPath.generic_string();

        return relativePath.generic_string();
    }

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

        return ToProjectRelativePath(std::filesystem::path(filePath));
    }

    std::string OpenTileSetAssetPath()
    {
        char filePath[MAX_PATH] = {};

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

        return ToProjectRelativePath(std::filesystem::path(filePath));
    }

    std::string SaveTileSetAssetPath()
    {
        char filePath[MAX_PATH] = {};

        OPENFILENAMEA dialog{};
        dialog.lStructSize = sizeof(dialog);
        dialog.lpstrFilter =
            "Tile Set Assets\0*.tileset.json\0"
            "JSON Files\0*.json\0"
            "All Files\0*.*\0";
        dialog.lpstrFile = filePath;
        dialog.nMaxFile = MAX_PATH;
        dialog.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
        dialog.lpstrDefExt = "json";
        dialog.lpstrTitle = "Save Tile Set Asset";

        if (!GetSaveFileNameA(&dialog))
            return {};

        std::filesystem::path selectedPath = std::filesystem::path(filePath);
        if (selectedPath.extension() != ".json" || selectedPath.stem().extension() != ".tileset")
            selectedPath += ".tileset.json";

        return ToProjectRelativePath(selectedPath);
    }
#endif
}

void TileSetEditorPanel::SyncBuffersFromAsset()
{
    std::snprintf(
        m_TexturePathBuffer.data(),
        m_TexturePathBuffer.size(),
        "%s",
        m_TileSetAsset.GetTexturePath().c_str()
    );

    m_ColumnsDraft = static_cast<int>(m_TileSetAsset.GetColumns());
    m_RowsDraft = static_cast<int>(m_TileSetAsset.GetRows());

    int textureWidth = 0;
    int textureHeight = 0;
    if (TryGetImageSize(m_TileSetAsset.GetTexturePath(), textureWidth, textureHeight))
    {
        m_CellWidthDraft = std::max(1, textureWidth / std::max(1, m_ColumnsDraft));
        m_CellHeightDraft = std::max(1, textureHeight / std::max(1, m_RowsDraft));
    }
    else
    {
        m_CellWidthDraft = 32;
        m_CellHeightDraft = 32;
    }

    ClampSelectedTile();
}

void TileSetEditorPanel::ClampSelectedTile()
{
    const int maxTileID = std::max(0, static_cast<int>(m_TileSetAsset.GetTileCount()) - 1);
    m_SelectedTileID = std::clamp(m_SelectedTileID, 0, maxTileID);
}

void TileSetEditorPanel::ApplyCellSizeFromTexture()
{
    m_CellWidthDraft = std::max(1, m_CellWidthDraft);
    m_CellHeightDraft = std::max(1, m_CellHeightDraft);

    int textureWidth = 0;
    int textureHeight = 0;
    if (!TryGetImageSize(m_TileSetAsset.GetTexturePath(), textureWidth, textureHeight))
        return;

    m_ColumnsDraft = std::max(1, textureWidth / m_CellWidthDraft);
    m_RowsDraft = std::max(1, textureHeight / m_CellHeightDraft);
    m_TileSetAsset.SetGrid(
        static_cast<uint32_t>(m_ColumnsDraft),
        static_cast<uint32_t>(m_RowsDraft)
    );
    ClampSelectedTile();
}

void TileSetEditorPanel::DrawAtlasPicker(VulkanRenderer& renderer)
{
    const std::string texturePath = m_TileSetAsset.GetTexturePath();
    if (texturePath.empty())
    {
        ImGui::TextDisabled("No tileset texture selected.");
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

    const uint32_t columns = std::max<uint32_t>(1, m_TileSetAsset.GetColumns());
    const uint32_t rows = std::max<uint32_t>(1, m_TileSetAsset.GetRows());

    ImGui::SliderFloat("Preview Scale", &m_PreviewScale, 0.25f, 8.0f, "%.2fx");

    const ImVec2 imageSize{
        static_cast<float>(texture->GetWidth()) * m_PreviewScale,
        static_cast<float>(texture->GetHeight()) * m_PreviewScale
    };

    ImGui::BeginChild("TileSetAtlasPreview", ImVec2(0.0f, 320.0f), true, ImGuiWindowFlags_HorizontalScrollbar);
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

    const int selectedColumn = m_SelectedTileID % static_cast<int>(columns);
    const int selectedRow = m_SelectedTileID / static_cast<int>(columns);

    const ImVec2 selectionMin{
        imageMin.x + cellWidth * static_cast<float>(selectedColumn),
        imageMin.y + cellHeight * static_cast<float>(selectedRow)
    };
    const ImVec2 selectionMax{
        selectionMin.x + cellWidth,
        selectionMin.y + cellHeight
    };
    drawList->AddRect(selectionMin, selectionMax, IM_COL32(255, 200, 0, 255), 0.0f, 0, 3.0f);

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        const ImVec2 mouse = ImGui::GetMousePos();
        const int column = static_cast<int>((mouse.x - imageMin.x) / cellWidth);
        const int row = static_cast<int>((mouse.y - imageMin.y) / cellHeight);

        if (column >= 0 && column < static_cast<int>(columns) &&
            row >= 0 && row < static_cast<int>(rows))
        {
            m_SelectedTileID = row * static_cast<int>(columns) + column;
        }
    }

    ImGui::EndChild();
}

void TileSetEditorPanel::Draw(VulkanRenderer& renderer)
{
    ImGui::Begin("Tile Set Editor");

    ImGui::SeparatorText("Asset");

    if (ImGui::InputText("Asset Path", m_AssetPathBuffer.data(), m_AssetPathBuffer.size()))
    {
    }

#ifdef _WIN32
    if (ImGui::Button("Browse Asset..."))
    {
        const std::string pickedPath = OpenTileSetAssetPath();
        if (!pickedPath.empty())
        {
            std::snprintf(
                m_AssetPathBuffer.data(),
                m_AssetPathBuffer.size(),
                "%s",
                pickedPath.c_str()
            );
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Save As..."))
    {
        const std::string pickedPath = SaveTileSetAssetPath();
        if (!pickedPath.empty())
        {
            std::snprintf(
                m_AssetPathBuffer.data(),
                m_AssetPathBuffer.size(),
                "%s",
                pickedPath.c_str()
            );
            m_TileSetAsset.SaveToFile(pickedPath);
        }
    }
#endif

    if (ImGui::Button("New"))
    {
        m_TileSetAsset = TileSetAsset{};
        std::snprintf(m_AssetPathBuffer.data(), m_AssetPathBuffer.size(), "%s", "");
        SyncBuffersFromAsset();
    }

    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        if (m_AssetPathBuffer[0] != '\0' && m_TileSetAsset.LoadFromFile(m_AssetPathBuffer.data()))
            SyncBuffersFromAsset();
    }

    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        if (m_AssetPathBuffer[0] != '\0')
            m_TileSetAsset.SaveToFile(m_AssetPathBuffer.data());
    }

    ImGui::SeparatorText("Tileset");

    if (ImGui::InputText("Texture Path", m_TexturePathBuffer.data(), m_TexturePathBuffer.size()))
    {
        m_TileSetAsset.SetTexturePath(m_TexturePathBuffer.data());
        ApplyCellSizeFromTexture();
    }

#ifdef _WIN32
    ImGui::SameLine();
    if (ImGui::Button("Browse Texture..."))
    {
        const std::string pickedPath = OpenTilesetTexturePath();
        if (!pickedPath.empty())
        {
            std::snprintf(
                m_TexturePathBuffer.data(),
                m_TexturePathBuffer.size(),
                "%s",
                pickedPath.c_str()
            );
            m_TileSetAsset.SetTexturePath(m_TexturePathBuffer.data());
            ApplyCellSizeFromTexture();
        }
    }
#endif

    ImGui::InputInt("Atlas Cell Width", &m_CellWidthDraft);
    ImGui::InputInt("Atlas Cell Height", &m_CellHeightDraft);

    if (ImGui::Button("Apply Cell Size"))
        ApplyCellSizeFromTexture();

    ImGui::Text("Resolved Grid: %d x %d", m_ColumnsDraft, m_RowsDraft);

    int textureWidth = 0;
    int textureHeight = 0;
    if (TryGetImageSize(m_TileSetAsset.GetTexturePath(), textureWidth, textureHeight))
    {
        const bool widthEven = (textureWidth % std::max(1, m_CellWidthDraft)) == 0;
        const bool heightEven = (textureHeight % std::max(1, m_CellHeightDraft)) == 0;
        if (!widthEven || !heightEven)
        {
            ImGui::TextColored(
                ImVec4(1.0f, 0.75f, 0.2f, 1.0f),
                "Warning: atlas cell size does not evenly divide the texture."
            );
        }
    }

    ImGui::SeparatorText("Atlas");
    DrawAtlasPicker(renderer);

    ImGui::SeparatorText("Selected Tile");
    ClampSelectedTile();
    ImGui::Text("Selected Tile ID: %d", m_SelectedTileID);

    bool solid = m_TileSetAsset.IsTileSolid(static_cast<uint32_t>(m_SelectedTileID));
    if (ImGui::Checkbox("Solid", &solid))
        m_TileSetAsset.SetTileSolid(static_cast<uint32_t>(m_SelectedTileID), solid);

    ImGui::End();
}
