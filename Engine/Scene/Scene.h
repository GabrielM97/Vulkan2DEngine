#pragma once

#include "GameObject.h"

#include <vector>

class VulkanRenderer;

class Scene
{
public:
    GameObject& CreateGameObject();

    void Render(VulkanRenderer& renderer);

    std::vector<GameObject>& GetGameObjects();
    const std::vector<GameObject>& GetGameObjects() const;

private:
    void SortForRendering();

private:
    std::vector<GameObject> m_GameObjects;
};
