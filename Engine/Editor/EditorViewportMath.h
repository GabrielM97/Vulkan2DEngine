#pragma once

#include <glm/vec2.hpp>

#include "Editor/SceneViewportPanel.h"

class Scene;

namespace EditorViewportMath
{
    glm::vec2 ScreenToWorld(
        Scene& scene,
        const glm::vec2& screenPosition,
        const SceneViewportState& viewportState);
}
