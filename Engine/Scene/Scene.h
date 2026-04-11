#pragma once

#include "Camera2D.h"
#include "GameObject.h"

#include <memory>
#include <string>
#include <vector>

struct CameraCommand
{
    float moveX = 0.0f;
    float moveY = 0.0f;
    float zoomDelta = 0.0f;
};

class IRenderer2D;

class Scene
{
public:
    GameObject& CreateGameObject(const std::string& name = "GameObject");

    void Render(IRenderer2D& renderer);
    void Update(float deltaTime);
    void UpdateCamera(const CameraCommand& command, float deltaTime);

    void DestroyGameObject(GameObject& object);
    size_t GetGameObjectCount() const;
    GameObject* GetGameObject(size_t index);
    const GameObject* GetGameObject(size_t index) const;

    GameObject* FindGameObjectByName(const std::string& name);
    const GameObject* FindGameObjectByName(const std::string& name) const;
    
    std::vector<std::unique_ptr<GameObject>>& GetGameObjects();
    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

    Camera2D& GetCamera() { return m_Camera; }
    const Camera2D& GetCamera() const { return m_Camera; }

private:
    void SortForRendering();
    void DestroyPendingGameObjects();

private:
    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
    Camera2D m_Camera;
};