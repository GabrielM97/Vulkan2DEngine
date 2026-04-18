#include "Editor/EditorViewportMath.h"

#include "Scene/Scene.h"

namespace EditorViewportMath
{
    glm::vec2 ScreenToWorld(
        Scene& scene,
        const glm::vec2& screenPosition,
        const SceneViewportState& viewportState)
    {
        const glm::vec2 viewportMin{
            viewportState.contentMin.x,
            viewportState.contentMin.y
        };

        const glm::vec2 viewportLocal = screenPosition - viewportMin;

        return scene.GetCamera().ScreenToWorld(
            viewportLocal,
            static_cast<float>(viewportState.width),
            static_cast<float>(viewportState.height)
        );
    }
}
