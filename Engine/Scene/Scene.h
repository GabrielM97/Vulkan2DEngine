#pragma once

#include "GameObject.h"

#include <memory>
#include <string>
#include <vector>

class VulkanRenderer;

class Scene
{
public:
    GameObject& CreateGameObject(const std::string& name = "GameObject");
    
    void Render(VulkanRenderer& renderer);
    void Update(float deltaTime);
    
    void DestroyGameObject(GameObject& object);
    
    std::vector<std::unique_ptr<GameObject>>& GetGameObjects();
    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

private:
    void SortForRendering();
    
    void DestroyPendingGameObjects();

private:
    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
};
