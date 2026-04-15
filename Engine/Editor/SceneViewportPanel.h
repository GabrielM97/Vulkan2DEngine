#pragma once

#include <cstdint>
#include <imgui.h>

struct SceneViewportState
{
    uint32_t width = 0;
    uint32_t height = 0;
    bool hovered = false;
    bool focused = false;
    bool visible = false;
    ImVec2 contentMin{0.0f, 0.0f};
    ImVec2 contentMax{0.0f, 0.0f};
};

class SceneViewportPanel
{
public:
    void Draw(ImTextureID textureID);
    const SceneViewportState& GetState() const { return m_State; }

private:
    SceneViewportState m_State;
};